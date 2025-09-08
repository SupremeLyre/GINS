/*------------------------------------------------------------------------------
 * pntpos.c : standard positioning
 *
 *          Copyright (C) 2007-2015 by T.TAKASU, All rights reserved.
 *
 * version : $Revision:$ $Date:$
 * history : 2010/07/28 1.0  moved from rtkcmn.c
 *                           changed api:
 *                               pntpos()
 *                           deleted api:
 *                               pntvel()
 *           2011/01/12 1.1  add option to include unhealthy satellite
 *                           reject duplicated observation data
 *                           changed api: ionocorr()
 *           2011/11/08 1.2  enable snr mask for single-mode (rtklib_2.4.1_p3)
 *           2012/12/25 1.3  add variable snr mask
 *           2014/05/26 1.4  support galileo and beidou
 *           2015/03/19 1.5  fix bug on ionosphere correction for GLO and BDS
 *-----------------------------------------------------------------------------*/
#include "rtklib.h"

/* constants -----------------------------------------------------------------*/

#define SQR(x) ((x) * (x))

#define NX (3 + 4) /* # of estimated parameters */

#define MAXITR 10     /* max number of iteration for point pos */
#define ERR_ION 5.0   /* ionospheric delay std (m) */
#define ERR_TROP 3.0  /* tropspheric delay std (m) */
#define ERR_SAAS 0.3  /* saastamoinen model error std (m) */
#define ERR_BRDCI 0.5 /* broadcast iono model error factor */
#define ERR_CBIAS 0.3 /* code bias error std (m) */
#define REL_HUMI 0.5  /* relative humidity for saastamoinen model */

/* get group delay parameter (m) ---------------------------------------------*/
static double gettgd(int sat, const nav_t *nav, int type)
{
    int i, sys = satsys(sat, NULL);

    if (sys == SYS_GLO)
    {
        for (i = 0; i < nav->ng; i++)
        {
            if (nav->geph[i].sat == sat)
                break;
        }
        return (i >= nav->ng) ? 0.0 : -nav->geph[i].dtaun * CLIGHT;
    }
    else
    {
        for (i = 0; i < nav->n; i++)
        {
            if (nav->eph[i].sat == sat)
                break;
        }
        return (i >= nav->n) ? 0.0 : nav->eph[i].tgd[type] * CLIGHT;
    }
}
/* test SNR mask -------------------------------------------------------------*/
static int snrmask(const obsd_t *obs, const double *azel, const prcopt_t *opt)
{
    if (testsnr(0, 0, azel[1], obs->SNR[0] * SNR_UNIT, &opt->snrmask))
    {
        return 0;
    }
    if (opt->ionoopt == IONOOPT_IFLC)
    {
        if (testsnr(0, 1, azel[1], obs->SNR[1] * SNR_UNIT, &opt->snrmask))
        {
            return 0;
        }
    }
    return 1;
}
/* psendorange with code bias correction -------------------------------------*/
static double prange(const obsd_t *obs, const nav_t *nav, const double *azel, int iter, const prcopt_t *opt,
                     double *var)
{
    int k, sat, sys, prn, orb;
    double P1, P2, gamma, b1, b2, freq[NFREQ] = {0};

    sat = obs->sat;
    sys = satsys(sat, &prn);
    satorb(obs->sat, &orb);
    P1 = obs->P[0];
    P2 = obs->P[1];
    *var = 0.0;

    for (k = 0; k < NFREQ; k++)
        freq[k] = sat2freq(obs->sat, obs->code[k], nav);

    if ((P1 == 0.0 || freq[0] == 0.0) || (opt->ionoopt == IONOOPT_IFLC && (P2 == 0.0 || freq[1] == 0.0)))
        return 0.0;

    /* P1-C1,P2-C2 DCB correction */
    if (sys == SYS_GPS || sys == SYS_GLO)
    {
        if (obs->code[0] == CODE_L1C)
            P1 += nav->cbias[sat - 1][1]; /* C1->P1 */
        if (obs->code[1] == CODE_L2C)
            P2 += nav->cbias[sat - 1][2]; /* C2->P2 */
    }
    if (opt->ionoopt == IONOOPT_IFLC)
    { /* dual-frequency */

        if (sys == SYS_GPS)
        { /* L1-L2,G1-G2 */
            gamma = SQR(freq[0] / freq[1]);
            return (P2 - gamma * P1) / (1.0 - gamma);
        }
        else if (sys == SYS_GLO)
        { /* G1-G2 */
            gamma = SQR(freq[0] / freq[1]);
            return (P2 - gamma * P1) / (1.0 - gamma);
        }
        else if (sys == SYS_GAL)
        { // E1-E5a
            gamma = SQR(freq[0] / freq[1]);
            return (P2 - gamma * P1) / (1.0 - gamma);
        }
        else if (sys == SYS_CMP)
        { // BRDC:B3	SP3:B2I-B6I
            if (prn < 19)
            {
                P1 += bd2smp(orb, azel, 0);
                P2 += bd2smp(orb, azel, 1);
            }
            gamma = SQR(freq[0] / freq[1]);
            b1 = (opt->sateph == EPHOPT_BRDC) ? gettgd(sat, nav, 0) : 0.0; /* TGD_B1I */
            P1 -= b1;
            return (P2 - gamma * P1) / (1.0 - gamma);
        }
    }
    else
    { /* single-frequency */
        *var = SQR(ERR_CBIAS);

        if (sys & SYS_GPS)
        {
            b1 = gettgd(sat, nav, 0); /* TGD (m) */
            return P1 - b1;
        }
        else if (sys & SYS_GLO)
        {
            gamma = SQR(freq[0] / freq[1]);
            b1 = gettgd(sat, nav, 0); /* -dtaun (m) */
            return P1 - b1 / (gamma - 1.0);
        }
        else if (sys & SYS_GAL)
        {
            b1 = gettgd(sat, nav, 0); /* BGD_E1E5a */
            return P1 - b1;
        }
        else if (sys & SYS_CMP)
        { // BRDC:B3	SP3:B2I-B6I
            if (prn < 19)
            {
                P1 += bd2smp(orb, azel, 0);
            }
            gamma = SQR(freq[0] / freq[1]);
            b1 = (opt->sateph == EPHOPT_BRDC) ? gettgd(sat, nav, 0) : gettgd(sat, nav, 0) / (1 - gamma); /* TGD_B1I */
            return P1 - b1;
        }
    }
    return P1;
}
/* pseudorange residuals -----------------------------------------------------*/
static int rescode(int iter, const obsd_t *obs, int n, double *rs, const double *dts, const double *vare,
                   const int *svh, const nav_t *nav, const double *x, const prcopt_t *opt, double *v, double *H,
                   double *var, double *azel, int *vsat, double *resp, int *ns, int *exc)
{
    int i, j, nv = 0, sat, sys, prn = 0, mask[4] = {0};
    double r, dion, dtrp, vmeas, vion, vtrp, rr[3], pos[3], e[3], P, freq1;

    trace(3, "resprng : n=%d\n", n);

    for (i = 0; i < 3; i++)
        rr[i] = x[i];

    ecef2pos(rr, pos);

    for (i = *ns = 0; i < n && i < MAXOBS; i++)
    {
        vsat[i] = 0;
        azel[i * 2] = azel[1 + i * 2] = resp[i] = 0.0;
        sat = obs[i].sat;

        if (!(sys = satsys(sat, &prn)))
            continue;
        // if (sys&SYS_CMP && ((prn < 19) || (prn > 46))) continue;

        /* reject duplicated observation data */
        if (i < n - 1 && i < MAXOBS - 1 && sat == obs[i + 1].sat)
        {
            trace(2, "duplicated observation data %s sat=%2d\n", time_str(obs[i].time, 3), sat);
            i++;
            continue;
        }
        /* excluded satellite? */
        if (satexclude(sat, svh[i], opt))
            continue;

        /* exclude obs */
        if (exc[i])
            continue;

        /* geometric distance/azimuth/elevation angle */
        if ((r = geodist(rs + i * 6, rr, e)) <= 0.0)
            continue;

        dion = dtrp = vion = vtrp = 0.0;
        if (iter > 0)
        {
            /* test elevation mask */
            if (satazel(pos, e, azel + i * 2) < opt->elmin)
                continue;

            /* test SNR mask */
            if (!snrmask(obs + i, azel + i * 2, opt))
                continue;

            /* tropospheric corrections */
            if (!tropcorr(obs[i].time, pos, azel + i * 2, opt, NULL, NULL, nav, &dtrp, &vtrp))
                continue;

            /* ionospheric corrections */
            if (!ionocorr(obs[i].time, pos, azel + i * 2, opt, obs[i].sat, NULL, nav, &dion, &vion))
                continue;
            if ((freq1 = sat2freq(sat, obs[i].code[0], nav)) == 0.0)
                continue;
            dion *= SQR(FREQ1 / freq1);
            vion *= SQR(FREQ1 / freq1);
        }

        /* psudorange with code bias correction */
        if ((P = prange(obs + i, nav, azel + i * 2, iter, opt, &vmeas)) == 0.0)
            continue;

        /* pseudorange residual */
        v[nv] = P - (r - CLIGHT * dts[i * 2] + dion + dtrp);

        /* design matrix */
        for (j = 0; j < NX; j++)
            H[j + nv * NX] = j < 3 ? -e[j] : 0.0;

        /* time system and receiver bias offset correction */
        if (sys == SYS_GPS)
        {
            v[nv] -= x[3];
            H[3 + nv * NX] = 1.0;
            mask[0] = 1;
        }
        else if (sys == SYS_GLO)
        {
            v[nv] -= x[4];
            H[4 + nv * NX] = 1.0;
            mask[1] = 1;
        }
        else if (sys == SYS_GAL)
        {
            v[nv] -= x[5];
            H[5 + nv * NX] = 1.0;
            mask[2] = 1;
        }
        else if (sys == SYS_CMP)
        {
            v[nv] -= x[6];
            H[6 + nv * NX] = 1.0;
            mask[3] = 1;
        }
        else
            continue;

        vsat[i] = 1;
        resp[i] = v[nv];
        (*ns)++;

        /* error variance */
        var[nv++] = varerr(obs[i].sat, azel[1 + i * 2], 0, 1, opt) + vare[i] + vmeas + vion + vtrp;

        trace(2, "iter=%2d sat=%2d azel=%5.1f %4.1f res=%7.3f sig=%5.3f\n", iter, obs[i].sat, azel[i * 2] * R2D,
              azel[1 + i * 2] * R2D, resp[i], sqrt(var[nv - 1]));
    }
    /* constraint to avoid rank-deficient */
    for (i = 0; i < 4; i++)
    {
        if (mask[i])
            continue;
        v[nv] = 0.0;
        for (j = 0; j < NX; j++)
            H[j + nv * NX] = j == i + 3 ? 1.0 : 0.0;
        var[nv++] = 0.001;
    }
    return nv;
}
/* validate solution ---------------------------------------------------------*/
static int valsol(const double *azel, const int *vsat, int n, const prcopt_t *opt, const double *v, int nv, int nx,
                  char *msg, int *nsat, double *pdop)
{
    double azels[MAXOBS * 2], dop[4], vv;
    int i, ns;

    trace(3, "valsol  : n=%d nv=%d\n", n, nv);

    /* chi-square validation of residuals */
    vv = dot(v, v, nv);
    if (nv > nx && vv > chisqr[nv - nx - 1])
    {
        sprintf(msg, "chi-square error nv=%d vv=%.1f cs=%.1f", nv, vv, chisqr[nv - nx - 1]);
        /* return 0; */ /* threshold too strict for all use cases, report error but continue on */
    }
    /* large gdop check */
    for (i = ns = 0; i < n; i++)
    {
        if (!vsat[i])
            continue;
        azels[ns * 2] = azel[i * 2];
        azels[1 + ns * 2] = azel[1 + i * 2];
        ns++;
    }
    dops(ns, azels, opt->elmin, dop);
    if (dop[0] <= 0.0 || dop[0] > opt->maxgdop)
    {
        sprintf(msg, "gdop error nv=%d gdop=%.1f", nv, dop[0]);
        return 0;
    }
    if (nsat)
        *nsat = ns;
    if (pdop)
        *pdop = dop[1];
    return 1;
}
/* estimate receiver position ------------------------------------------------*/
static int estpos(const obsd_t *obs, int n, double *rs, const double *dts, const double *vare, const int *svh,
                  const nav_t *nav, const prcopt_t *opt, sol_t *sol, double *azel, int *vsat, double *resp, char *msg)
{
    double x[NX] = {0}, dx[NX], Q[NX * NX], *v, *H, *var, sig;
    int i, j, k, info, stat, nv, ns, *exc, ind, d;

    trace(3, "estpos  : n=%d\n", n);

    v = mat(n + 4, 1);
    H = mat(NX, n + 4);
    var = mat(n + 4, 1);
    exc = imat(n, 1);

    for (i = 0; i < 3; i++)
        x[i] = sol->rr[i];

    for (i = 0; i < n; i++)
        exc[i] = 0;

    for (i = 0; i < MAXITR; i++)
    {

        /* pseudorange residuals */
        nv = rescode(i, obs, n, rs, dts, vare, svh, nav, x, opt, v, H, var, azel, vsat, resp, &ns, exc);
        if (nv < NX)
        {
            sprintf(msg, "lack of valid sats ns=%d", nv);
            break;
        }
        /* weight by variance */
        for (j = 0; j < nv; j++)
        {
            sig = sqrt(var[j]);
            v[j] /= sig;
            for (k = 0; k < NX; k++)
                H[k + j * NX] /= sig;
        }
        /* least square estimation */
        if ((info = lsq(H, v, NX, nv, dx, Q)))
        {
            sprintf(msg, "lsq error info=%d", info);
            break;
        }
        for (j = 0; j < NX; j++)
            x[j] += dx[j];

        if (norm(dx, NX) < 1E-4)
        {
            /* 后验残差 */
            double max = 0.0;
            for (j = k = max = 0; j < n; j++)
            {
                if (vsat[j])
                {
                    if (fabs(resp[k]) > max)
                    {
                        max = resp[k];
                        ind = j;
                    }
                    k++;
                }
            }
            for (j = d = 0; j < n; j++)
                if (exc[j])
                    d++;
            if ((k - d) > 7 && max > 20)
            {
                exc[ind] = 1;
                continue;
            } // new add by zh in 23-08-27

            sol->type = 0;
            sol->time = timeadd(obs[0].time, -x[3] / CLIGHT);
            sol->dtr[0] = x[3] / CLIGHT; /* gps time offset (s) */
            sol->dtr[1] = x[4] / CLIGHT; /* glo time offset (s) */
            sol->dtr[2] = x[5] / CLIGHT; /* gal time offset (s) */
            sol->dtr[3] = x[6] / CLIGHT; /* bds time offset (s) */
            for (j = 0; j < 6; j++)
                sol->rr[j] = j < 3 ? x[j] : 0.0;
            for (j = 0; j < 3; j++)
                sol->qr[j] = (float)Q[j + j * NX];
            sol->qr[3] = (float)Q[1];      /* cov xy */
            sol->qr[4] = (float)Q[2 + NX]; /* cov yz */
            sol->qr[5] = (float)Q[2];      /* cov zx */
            sol->ns = (unsigned char)ns;
            sol->age = sol->ratio = 0.0;

            /* validate solution */
            int nsat = 0;
            double pdop = 0.0;
            if ((stat = valsol(azel, vsat, n, opt, v, nv, NX, msg, &nsat, &pdop)))
            {
                sol->stat = opt->sateph == EPHOPT_SBAS ? SOLQ_SBAS : SOLQ_SINGLE;
                sol->nsat = nsat;
                sol->pdop = pdop;
            }

            free(v);
            free(H);
            free(var);
            return stat;
        }
    }

    if (i >= MAXITR)
        sprintf(msg, "iteration divergent i=%d", i);

    free(v);
    free(H);
    free(var);

    return 0;
}
/* range rate residuals ------------------------------------------------------*/
static int resdop(const obsd_t *obs, int n, const double *rs, const double *dts, const nav_t *nav, const double *rr,
                  const double *x, const double *azel, const int *vsat, double err, double *v, double *H)
{
    double freq, rate, pos[3], E[9], a[3], e[3], vs[3], cosel, sig;
    int i, j, nv = 0;

    trace(3, "resdop  : n=%d\n", n);

    ecef2pos(rr, pos);
    xyz2enu(pos, E);

    for (i = 0; i < n && i < MAXOBS; i++)
    {

        freq = sat2freq(obs[i].sat, obs[i].code[0], nav);

        if (obs[i].D[0] == 0.0 || freq == 0.0 || !vsat[i] || norm(rs + 3 + i * 6, 3) <= 0.0)
        {
            continue;
        }
        /* LOS (line-of-sight) vector in ECEF */
        cosel = cos(azel[1 + i * 2]);
        a[0] = sin(azel[i * 2]) * cosel;
        a[1] = cos(azel[i * 2]) * cosel;
        a[2] = sin(azel[1 + i * 2]);
        matmul("TN", 3, 1, 3, 1.0, E, a, 0.0, e);

        /* satellite velocity relative to receiver in ECEF */
        for (j = 0; j < 3; j++)
        {
            vs[j] = rs[j + 3 + i * 6] - x[j];
        }
        /* range rate with earth rotation correction */
        rate =
            dot(vs, e, 3) +
            OMGE / CLIGHT * (rs[4 + i * 6] * rr[0] + rs[1 + i * 6] * x[0] - rs[3 + i * 6] * rr[1] - rs[i * 6] * x[1]);

        /* Std of range rate error (m/s) */
        sig = (err <= 0.0) ? 1.0 : err * CLIGHT / freq;

        /* range rate residual (m/s) */
        // um980 doppler*-1
        v[nv] = (obs[i].D[0] * CLIGHT / freq - (rate + x[3] - CLIGHT * dts[1 + i * 2])) / sig;

        /* design matrix */
        for (j = 0; j < 4; j++)
        {
            H[j + nv * 4] = ((j < 3) ? -e[j] : 1.0) / sig;
        }
        nv++;
    }
    return nv;
}
/* estimate receiver velocity ------------------------------------------------*/
static void estvel(const obsd_t *obs, int n, const double *rs, const double *dts, const nav_t *nav, const prcopt_t *opt,
                   sol_t *sol, const double *azel, const int *vsat)
{
    double x[4] = {0}, dx[4], Q[16], *v, *H;
    double err = opt->err[4]; /* Doppler error (Hz) */
    int i, j, nv;

    trace(3, "estvel  : n=%d\n", n);

    v = mat(n, 1);
    H = mat(4, n);

    for (i = 0; i < MAXITR; i++)
    {

        /* range rate residuals (m/s) */
        if ((nv = resdop(obs, n, rs, dts, nav, sol->rr, x, azel, vsat, err, v, H)) < 4)
        {
            break;
        }
        /* least square estimation */
        if (lsq(H, v, 4, nv, dx, Q))
            break;

        for (j = 0; j < 4; j++)
            x[j] += dx[j];

        if (norm(dx, 4) < 1E-6)
        {
            matcpy(sol->rr + 3, x, 3, 1);
            sol->qv[0] = (float)Q[0];  /* xx */
            sol->qv[1] = (float)Q[5];  /* yy */
            sol->qv[2] = (float)Q[10]; /* zz */
            sol->qv[3] = (float)Q[1];  /* xy */
            sol->qv[4] = (float)Q[6];  /* yz */
            sol->qv[5] = (float)Q[2];  /* zx */
            break;
        }
    }
    free(v);
    free(H);
}
/* single-point positioning ---------------------------------------------------- */
extern int pntpos(const obsd_t *obs, int n, const nav_t *nav, const prcopt_t *opt, sol_t *sol, double *azel,
                  ssat_t *ssat, char *msg)
{
    prcopt_t opt_ = *opt;
    double *rs, *dts, *var, *azel_, *resp;
    int i, stat, vsat[MAXOBS] = {0}, svh[MAXOBS];

    sol->stat = SOLQ_NONE;

    if (n <= 0)
    {
        strcpy(msg, "no observation data");
        return 0;
    }
    if (!ssat)
    {
        strcpy(msg, "spp ssat is null");
        return 0;
    }

    trace(3, "pntpos  : tobs=%s n=%d\n", time_str(obs[0].time, 3), n);

    sol->time = obs[0].time;
    msg[0] = '\0';

    rs = mat(6, n);
    dts = mat(2, n);
    var = mat(1, n);
    azel_ = zeros(2, n);
    resp = mat(1, n);

    if (opt_.mode != PMODE_SINGLE)
    {

        opt_.sateph = EPHOPT_BRDC;
        opt_.ionoopt = (opt_.nf == 1) ? IONOOPT_BRDC : IONOOPT_IFLC;
        opt_.tropopt = TROPOPT_SAAS;
    }
    /* pseudo & doppler error detect */

    /* satellite positons, velocities and clocks */
    satposs(sol->time, obs, n, nav, opt_.sateph, rs, dts, var, svh);

    /* estimate receiver position with pseudorange */
    stat = estpos(obs, n, rs, dts, var, svh, nav, &opt_, sol, azel_, vsat, resp, msg);

    /* estimate receiver velocity with doppler */
    if (stat)
        estvel(obs, n, rs, dts, nav, &opt_, sol, azel_, vsat);

    if (azel)
    {
        for (i = 0; i < n * 2; i++)
            azel[i] = azel_[i];
    }
    if (ssat)
    {
        for (i = 0; i < MAXSAT; i++)
        {
            ssat[i].vs = 0;
            ssat[i].azel[0] = ssat[i].azel[1] = 0.0;
            ssat[i].resp[0] = 0.0;
            ssat[i].respp = 0.0; // new add
        }
        for (i = 0; i < n; i++)
        {
            ssat[obs[i].sat - 1].azel[0] = azel_[i * 2];
            ssat[obs[i].sat - 1].azel[1] = azel_[1 + i * 2];
            if (!vsat[i] || fabs(resp[i]) > opt_.threscheck[4])
                continue;
            ssat[obs[i].sat - 1].respp = resp[i]; // new add
            ssat[obs[i].sat - 1].vs = 1;
            ssat[obs[i].sat - 1].resp[0] = resp[i];
        }
    }
    free(rs);
    free(dts);
    free(var);
    free(azel_);
    free(resp);
    return stat;
}