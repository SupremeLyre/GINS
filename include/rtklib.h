/*------------------------------------------------------------------------------
 * rtklib.h : rtklib constants, types and function prototypes  ��������
 *
 *          Copyright (C) 2007-2016 by T.TAKASU, All rights reserved.
 *
 * options : -DENAGLO   enable GLONASS
 *           -DENAGAL   enable Galileo
 *           -DENAQZS   enable QZSS
 *           -DENACMP   enable BeiDou
 *           -DENAIRN   enable IRNSS
 *           -DNFREQ=n  set number of obs codes/frequencies
 *           -DNEXOBS=n set number of extended obs codes
 *           -DMAXOBS=n set max number of obs data in an epoch
 *           -DEXTLEX   enable QZSS LEX extension
 *           -DWIN32    use WIN32 API
 *           -DWIN_DLL  generate library as Windows DLL
 *
 * version : $Revision: 1.1 $ $Date: 2008/07/17 21:48:06 $
 * history : 2007/01/13 1.0  rtklib ver.1.0.0
 *           2007/03/20 1.1  rtklib ver.1.1.0
 *           2008/07/15 1.2  rtklib ver.2.1.0
 *           2008/10/19 1.3  rtklib ver.2.1.1
 *           2009/01/31 1.4  rtklib ver.2.2.0
 *           2009/04/30 1.5  rtklib ver.2.2.1
 *           2009/07/30 1.6  rtklib ver.2.2.2
 *           2009/12/25 1.7  rtklib ver.2.3.0
 *           2010/07/29 1.8  rtklib ver.2.4.0
 *           2011/05/27 1.9  rtklib ver.2.4.1
 *           2013/03/28 1.10 rtklib ver.2.4.2
 *           2016/01/26 1.11 rtklib ver.2.4.3
 *-----------------------------------------------------------------------------*/
#ifndef RTKLIB_H
#define RTKLIB_H
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#include <winsock2.h>
#else
#include <pthread.h>
#endif
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef WIN_DLL
#define EXPORT __declspec(dllexport) /* for Windows DLL */
#else
#define EXPORT
#endif

    /* constants -----------------------------------------------------------------*/

#define VER_RTKLIB "2.4.3 demo5" /* library version */

#define PATCH_LEVEL "b29d" /* patch level */

#define COPYRIGHT_RTKLIB "Copyright (C) 2007-2018 T.Takasu\nAll rights reserved."

#define PI 3.1415926535897932  /* pi */
#define D2R (PI / 180.0)       /* deg to rad */
#define R2D (180.0 / PI)       /* rad to deg */
#define CLIGHT 299792458.0     /* speed of light (m/s) */
#define SC2RAD 3.1415926535898 /* semi-circle to radian (IS-GPS) */
#define AU 149597870691.0      /* 1 AU (m) */
#define AS2R (D2R / 3600.0)    /* arc sec to radian */

#define OMGE 7.2921151467E-5 /* earth angular velocity (IS-GPS) (rad/s) */

#define RE_WGS84 6378137.0             /* earth semimajor axis (WGS84) (m) */
#define FE_WGS84 (1.0 / 298.257223563) /* earth flattening (WGS84) */

#define HION 350000.0 /* ionosphere height (m) */

#define MAXFREQ 7 /* max NFREQ */

#define ISZERO(x) ((fabs(x) < 1E-8) ? 1 : 0)

#define FREQ1 1.57542E9       /* L1/E1  frequency (Hz) */
#define FREQ2 1.22760E9       /* L2     frequency (Hz) */
#define FREQ5 1.17645E9       /* L5/E5a frequency (Hz) */
#define FREQ6 1.27875E9       /* E6/LEX frequency (Hz) */
#define FREQ7 1.20714E9       /* E5b    frequency (Hz) */
#define FREQ8 1.191795E9      /* E5a+b  frequency (Hz) */
#define FREQ9 2.492028E9      /* S      frequency (Hz) */
#define FREQ1_GLO 1.60200E9   /* GLONASS G1 base frequency (Hz) */
#define DFRQ1_GLO 0.56250E6   /* GLONASS G1 bias frequency (Hz/n) */
#define FREQ2_GLO 1.24600E9   /* GLONASS G2 base frequency (Hz) */
#define DFRQ2_GLO 0.43750E6   /* GLONASS G2 bias frequency (Hz/n) */
#define FREQ3_GLO 1.202025E9  /* GLONASS G3 frequency (Hz) */
#define FREQ1a_GLO 1.600995E9 /* GLONASS G1a frequency (Hz) */
#define FREQ2a_GLO 1.248060E9 /* GLONASS G2a frequency (Hz) */
#define FREQ1_CMP 1.561098E9  /* BeiDou B1 frequency (Hz) */
#define FREQ2_CMP 1.20714E9   /* BeiDou B2 frequency (Hz) */
#define FREQ3_CMP 1.26852E9   /* BeiDou B3 frequency (Hz) */

#define EFACT_GPS 1.0                                      /* error factor: GPS */
#define EFACT_GPS_L5 10.0 /* error factor of GPS/QZS L5 */ // 10--5
#define EFACT_GLO 1.5 /* error factor: GLONASS */          // 1.5--1.0
#define EFACT_GAL 1.0                                      /* error factor: Galileo */
#define EFACT_QZS 1.0                                      /* error factor: QZSS */
#define EFACT_BD2 4.0                                      /* error factor: BeiDou */
#define EFACT_BD3 1.0                                      /* error factor: BeiDou-3 */
#define EFACT_IRN 1.5                                      /* error factor: IRNSS */
#define EFACT_SBS 3.0                                      /* error factor: SBAS */

#define SYS_NONE 0x00 /* navigation system: none */
#define SYS_GPS 0x01  /* navigation system: GPS */
#define SYS_SBS 0x02  /* navigation system: SBAS */
#define SYS_GLO 0x04  /* navigation system: GLONASS */
#define SYS_GAL 0x08  /* navigation system: Galileo */
#define SYS_QZS 0x10  /* navigation system: QZSS */
#define SYS_CMP 0x20  /* navigation system: BeiDou */
#define SYS_IRN 0x40  /* navigation system: IRNS */
#define SYS_LEO 0x80  /* navigation system: LEO */
#define SYS_ALL 0xFF  /* navigation system: all */

#define TSYS_GPS 0 /* time system: GPS time */
#define TSYS_UTC 1 /* time system: UTC */
#define TSYS_GLO 2 /* time system: GLONASS time */
#define TSYS_GAL 3 /* time system: Galileo time */
#define TSYS_QZS 4 /* time system: QZSS time */
#define TSYS_CMP 5 /* time system: BeiDou time */
#define TSYS_IRN 6 /* time system: IRNSS time */

#define SAT_NONE 0 /* satellite styple: none */
#define SAT_GEO 1  /* satellite styple: BDS-GEO */
#define SAT_IGSO 2 /* satellite styple: BDS-IGSO */
#define SAT_MEO 3  /* satellite styple: BDS-MEO */

#ifndef NFREQ
#define NFREQ 3 /* number of carrier frequencies */
#endif
#define NFREQGLO 2 /* number of carrier frequencies of GLONASS */

#ifndef NEXOBS
#define NEXOBS 0 /* number of extended obs codes */
#endif

#define SNR_UNIT 0.001 /* SNR unit (dBHz) */

#define MINPRNGPS 1                         /* min satellite PRN number of GPS */
#define MAXPRNGPS 32                        /* max satellite PRN number of GPS */
#define NSATGPS (MAXPRNGPS - MINPRNGPS + 1) /* number of GPS satellites */
#define NSYSGPS 1

#ifdef ENAGLO
#define MINPRNGLO 1                         /* min satellite slot number of GLONASS */
#define MAXPRNGLO 27                        /* max satellite slot number of GLONASS */
#define NSATGLO (MAXPRNGLO - MINPRNGLO + 1) /* number of GLONASS satellites */
#define NSYSGLO 1
#else
#define MINPRNGLO 0
#define MAXPRNGLO 0
#define NSATGLO 0
#define NSYSGLO 0
#endif
#ifdef ENAGAL
#define MINPRNGAL 1                         /* min satellite PRN number of Galileo */
#define MAXPRNGAL 36                        /* max satellite PRN number of Galileo */
#define NSATGAL (MAXPRNGAL - MINPRNGAL + 1) /* number of Galileo satellites */
#define NSYSGAL 1
#else
#define MINPRNGAL 0
#define MAXPRNGAL 0
#define NSATGAL 0
#define NSYSGAL 0
#endif
#ifdef ENAQZS
#define MINPRNQZS 193                       /* min satellite PRN number of QZSS */
#define MAXPRNQZS 199                       /* max satellite PRN number of QZSS */
#define MINPRNQZS_S 183                     /* min satellite PRN number of QZSS SAIF */
#define MAXPRNQZS_S 189                     /* max satellite PRN number of QZSS SAIF */
#define NSATQZS (MAXPRNQZS - MINPRNQZS + 1) /* number of QZSS satellites */
#define NSYSQZS 1
#else
#define MINPRNQZS 0
#define MAXPRNQZS 0
#define MINPRNQZS_S 0
#define MAXPRNQZS_S 0
#define NSATQZS 0
#define NSYSQZS 0
#endif
#ifdef ENACMP
#define MINPRNCMP 1                                           /* min satellite sat number of BeiDou */
#define MAXPRNCMP 62 /* max satellite sat number of BeiDou */ // 2021-8-17  36->62
#define NSATCMP (MAXPRNCMP - MINPRNCMP + 1)                   /* number of BeiDou satellites */
#define NSYSCMP 1
#else
#define MINPRNCMP 0
#define MAXPRNCMP 0
#define NSATCMP 0
#define NSYSCMP 0
#endif
#ifdef ENAIRN
#define MINPRNIRN 1                         /* min satellite sat number of IRNSS */
#define MAXPRNIRN 7                         /* max satellite sat number of IRNSS */
#define NSATIRN (MAXPRNIRN - MINPRNIRN + 1) /* number of IRNSS satellites */
#define NSYSIRN 1
#else
#define MINPRNIRN 0
#define MAXPRNIRN 0
#define NSATIRN 0
#define NSYSIRN 0
#endif
#ifdef ENALEO
#define MINPRNLEO 1                         /* min satellite sat number of LEO */
#define MAXPRNLEO 10                        /* max satellite sat number of LEO */
#define NSATLEO (MAXPRNLEO - MINPRNLEO + 1) /* number of LEO satellites */
#define NSYSLEO 1
#else
#define MINPRNLEO 0
#define MAXPRNLEO 0
#define NSATLEO 0
#define NSYSLEO 0
#endif
#define NSYS (NSYSGPS + NSYSGLO + NSYSGAL + NSYSQZS + NSYSCMP + NSYSIRN + NSYSLEO) /* number of systems */

#define MINPRNSBS 0 /* min satellite PRN number of SBAS */ // 120--0
#define MAXPRNSBS 0 /* max satellite PRN number of SBAS */ // 142--0
#define NSATSBS 0 /* number of SBAS satellites */          //(MAXPRNSBS-MINPRNSBS+1)--0

#define MAXSAT (NSATGPS + NSATGLO + NSATGAL + NSATQZS + NSATCMP + NSATIRN + NSATSBS + NSATLEO)
    /* max satellite number (1 to MAXSAT) */
#define MAXSTA 255

#ifndef MAXOBS
#define MAXOBS 96 /* max number of obs in an epoch */ // 2021-1-9 64->70
#endif
#define MAXRCV 200 /* max receiver number (1 to MAXRCV) */ // 2021-5-24 150->400
#define MAXOBSTYPE 64                                      /* max number of obs type in RINEX */
#ifdef OBS_100HZ
#define DTTOL 0.005 /* tolerance of time difference (s) */
#else
#define DTTOL 0.025 /* tolerance of time difference (s) */
#endif
#define MAXDTOE 7200.0      /* max time difference to GPS Toe (s) */
#define MAXDTOE_QZS 7200.0  /* max time difference to QZSS Toe (s) */
#define MAXDTOE_GAL 10800.0 /* max time difference to Galileo Toe (s) */
#define MAXDTOE_CMP 21600.0 /* max time difference to BeiDou Toe (s) */
#define MAXDTOE_GLO 1800.0  /* max time difference to GLONASS Toe (s) */
#define MAXDTOE_IRN 7200.0  /* max time difference to IRNSS Toe (s) */
#define MAXDTOE_SBS 360.0   /* max time difference to SBAS Toe (s) */
#define MAXDTOE_S 86400.0   /* max time difference to ephem toe (s) for other */
#define MAXGDOP 300.0       /* max GDOP */

#define INT_SWAP_TRAC 86400.0 /* swap interval of trace file (s) */
#define INT_SWAP_STAT 86400.0 /* swap interval of solution status file (s) */

#define MAXEXFILE 1024                                      /* max number of expanded files */
#define MAXSBSAGEF 30.0                                     /* max age of SBAS fast correction (s) */
#define MAXSBSAGEL 1800.0                                   /* max age of SBAS long term corr (s) */
#define MAXSBSURA 8                                         /* max URA of SBAS satellite */
#define MAXBAND 10                                          /* max SBAS band of IGP */
#define MAXNIGP 201                                         /* max number of IGP in SBAS band */
#define MAXNGEO 4                                           /* max number of GEO satellites */
#define MAXCOMMENT 10                                       /* max number of RINEX comments */
#define MAXSTRPATH 1024                                     /* max length of stream path */
#define MAXSTRMSG 1024                                      /* max length of stream message */
#define MAXSTRRTK 8                                         /* max number of stream in RTK server */
#define MAXSBSMSG 32                                        /* max number of SBAS msg in RTK server */
#define MAXSOLMSG 8191 /* max length of solution message */ //ͷ�ļ�����
#define MAXRAWLEN 8192                                      /* max length of receiver raw message */
#define MAXERRMSG 4096                                      /* max length of error/warning message */
#define MAXANT 64                                           /* max length of station name/antenna type */
#define MAXSOLBUF 256                                       /* max number of solution buffer */
#define MAXOBSBUF 128                                       /* max number of observation data buffer */
#define MAXNRPOS 16                                         /* max number of reference positions */
#define MAXLEAPS 64                                         /* max number of leap seconds table */
#define MAXGISLAYER 32                                      /* max number of GIS data layers */
#define MAXRCVCMD 4096                                      /* max length of receiver commands */

#define RNX2VER 2.10 /* RINEX ver.2 default output version */
#define RNX3VER 3.00 /* RINEX ver.3 default output version */

#define OBSTYPE_PR 0x01  /* observation type: pseudorange */
#define OBSTYPE_CP 0x02  /* observation type: carrier-phase */
#define OBSTYPE_DOP 0x04 /* observation type: doppler-freq */
#define OBSTYPE_SNR 0x08 /* observation type: SNR */
#define OBSTYPE_ALL 0xFF /* observation type: all */

#define FREQTYPE_L1 0x01  /* frequency type: L1/E1 */
#define FREQTYPE_L2 0x02  /* frequency type: L2/B1 */
#define FREQTYPE_L5 0x04  /* frequency type: L5/E5a/L3 */
#define FREQTYPE_L6 0x08  /* frequency type: E6/LEX/B3 */
#define FREQTYPE_L7 0x10  /* frequency type: E5b/B2 */
#define FREQTYPE_L8 0x20  /* frequency type: E5(a+b) */
#define FREQTYPE_L9 0x40  /* frequency type: S */
#define FREQTYPE_ALL 0xFF /* frequency type: all */

#define CODE_NONE 0 /* obs code: none or unknown */
#define CODE_L1C 1  /* obs code: L1C/A,G1C/A,E1C (GPS,GLO,GAL,QZS,SBS) */
#define CODE_L1P 2  /* obs code: L1P,G1P,B1P (GPS,GLO,BDS) */
#define CODE_L1W 3  /* obs code: L1 Z-track (GPS) */
#define CODE_L1Y 4  /* obs code: L1Y        (GPS) */
#define CODE_L1M 5  /* obs code: L1M        (GPS) */
#define CODE_L1N 6  /* obs code: L1codeless,B1codeless (GPS,BDS) */
#define CODE_L1S 7  /* obs code: L1C(D)     (GPS,QZS) */
#define CODE_L1L 8  /* obs code: L1C(P)     (GPS,QZS) */
#define CODE_L1E 9  /* (not used) */
#define CODE_L1A 10 /* obs code: E1A,B1A    (GAL,BDS) */
#define CODE_L1B 11 /* obs code: E1B        (GAL) */
#define CODE_L1X 12 /* obs code: E1B+C,L1C(D+P),B1D+P (GAL,QZS,BDS) */
#define CODE_L1Z 13 /* obs code: E1A+B+C,L1S (GAL,QZS) */
#define CODE_L2C 14 /* obs code: L2C/A,G1C/A (GPS,GLO) */
#define CODE_L2D 15 /* obs code: L2 L1C/A-(P2-P1) (GPS) */
#define CODE_L2S 16 /* obs code: L2C(M)     (GPS,QZS) */
#define CODE_L2L 17 /* obs code: L2C(L)     (GPS,QZS) */
#define CODE_L2X 18 /* obs code: L2C(M+L),B1_2I+Q (GPS,QZS,BDS) */
#define CODE_L2P 19 /* obs code: L2P,G2P    (GPS,GLO) */
#define CODE_L2W 20 /* obs code: L2 Z-track (GPS) */
#define CODE_L2Y 21 /* obs code: L2Y        (GPS) */
#define CODE_L2M 22 /* obs code: L2M        (GPS) */
#define CODE_L2N 23 /* obs code: L2codeless (GPS) */
#define CODE_L5I 24 /* obs code: L5I,E5aI   (GPS,GAL,QZS,SBS) */
#define CODE_L5Q 25 /* obs code: L5Q,E5aQ   (GPS,GAL,QZS,SBS) */
#define CODE_L5X 26 /* obs code: L5I+Q,E5aI+Q,L5B+C,B2aD+P (GPS,GAL,QZS,IRN,SBS,BDS) */
#define CODE_L7I 27 /* obs code: E5bI,B2bI  (GAL,BDS) */
#define CODE_L7Q 28 /* obs code: E5bQ,B2bQ  (GAL,BDS) */
#define CODE_L7X 29 /* obs code: E5bI+Q,B2bI+Q (GAL,BDS) */
#define CODE_L6A 30 /* obs code: E6A,B3A    (GAL,BDS) */
#define CODE_L6B 31 /* obs code: E6B        (GAL) */
#define CODE_L6C 32 /* obs code: E6C        (GAL) */
#define CODE_L6X 33 /* obs code: E6B+C,LEXS+L,B3I+Q (GAL,QZS,BDS) */
#define CODE_L6Z 34 /* obs code: E6A+B+C,L6D+E (GAL,QZS) */
#define CODE_L6S 35 /* obs code: L6S        (QZS) */
#define CODE_L6L 36 /* obs code: L6L        (QZS) */
#define CODE_L8I 37 /* obs code: E5abI      (GAL) */
#define CODE_L8Q 38 /* obs code: E5abQ      (GAL) */
#define CODE_L8X 39 /* obs code: E5abI+Q,B2abD+P (GAL,BDS) */
#define CODE_L2I 40 /* obs code: B1_2I      (BDS) */
#define CODE_L2Q 41 /* obs code: B1_2Q      (BDS) */
#define CODE_L6I 42 /* obs code: B3I        (BDS) */
#define CODE_L6Q 43 /* obs code: B3Q        (BDS) */
#define CODE_L3I 44 /* obs code: G3I        (GLO) */
#define CODE_L3Q 45 /* obs code: G3Q        (GLO) */
#define CODE_L3X 46 /* obs code: G3I+Q      (GLO) */
#define CODE_L1I 47 /* obs code: B1I        (BDS) (obsolute) */
#define CODE_L1Q 48 /* obs code: B1Q        (BDS) (obsolute) */
#define CODE_L5A 49 /* obs code: L5A SPS    (IRN) */
#define CODE_L5B 50 /* obs code: L5B RS(D)  (IRN) */
#define CODE_L5C 51 /* obs code: L5C RS(P)  (IRN) */
#define CODE_L9A 52 /* obs code: SA SPS     (IRN) */
#define CODE_L9B 53 /* obs code: SB RS(D)   (IRN) */
#define CODE_L9C 54 /* obs code: SC RS(P)   (IRN) */
#define CODE_L9X 55 /* obs code: SB+C       (IRN) */
#define CODE_L1D 56 /* obs code: B1D        (BDS) */
#define CODE_L5D 57 /* obs code: L5D(L5S),B2aD (QZS,BDS) */
#define CODE_L5P 58 /* obs code: L5P(L5S),B2aP (QZS,BDS) */
#define CODE_L5Z 59 /* obs code: L5D+P(L5S) (QZS) */
#define CODE_L6E 60 /* obs code: L6E        (QZS) */
#define CODE_L7D 61 /* obs code: B2bD       (BDS) */
#define CODE_L7P 62 /* obs code: B2bP       (BDS) */
#define CODE_L7Z 63 /* obs code: B2bD+P     (BDS) */
#define CODE_L8D 64 /* obs code: B2abD      (BDS) */
#define CODE_L8P 65 /* obs code: B2abP      (BDS) */
#define CODE_L4A 66 /* obs code: G1aL1OCd   (GLO) */
#define CODE_L4B 67 /* obs code: G1aL1OCd   (GLO) */
#define CODE_L4X 68 /* obs code: G1al1OCd+p (GLO) */
#define MAXCODE 68  /* max number of obs code */

#define PMODE_SINGLE 0       /* positioning mode: single */
#define PMODE_DGPS 1         /* positioning mode: DGPS/DGNSS */
#define PMODE_KINEMA 2       /* positioning mode: kinematic */
#define PMODE_STATIC 3       /* positioning mode: static */
#define PMODE_STATIC_START 4 /* positioning mode: static */
#define PMODE_MOVEB 5        /* positioning mode: moving-base */
#define PMODE_FIXED 6        /* positioning mode: fixed */
#define PMODE_PPP_KINEMA 7   /* positioning mode: PPP-kinemaric */
#define PMODE_PPP_STATIC 8   /* positioning mode: PPP-static */
#define PMODE_PPP_FIXED 9    /* positioning mode: PPP-fixed */

#define SOLF_LLH 0  /* solution format: lat/lon/height */
#define SOLF_XYZ 1  /* solution format: x/y/z-ecef */
#define SOLF_ENU 2  /* solution format: e/n/u-baseline */
#define SOLF_NMEA 3 /* solution format: NMEA-183 */
#define SOLF_STAT 4 /* solution format: solution status */
#define SOLF_GSIF 5 /* solution format: GSI F1/F2 */

#define SOLQ_NONE 0   /* solution status: no solution */
#define SOLQ_FIX 1    /* solution status: fix */
#define SOLQ_FLOAT 2  /* solution status: float */
#define SOLQ_SBAS 3   /* solution status: SBAS */
#define SOLQ_DGPS 4   /* solution status: DGPS/DGNSS */
#define SOLQ_SINGLE 5 /* solution status: single */
#define SOLQ_PPP 6    /* solution status: PPP */
#define SOLQ_DR 7     /* solution status: dead reconing */
#define MAXSOLQ 7     /* max number of solution status */

#define TIMES_GPST 0 /* time system: gps time */
#define TIMES_UTC 1  /* time system: utc */
#define TIMES_JST 2  /* time system: jst */

#define IONOOPT_OFF 0  /* ionosphere option: correction off */
#define IONOOPT_BRDC 1 /* ionosphere option: broadcast model */
#define IONOOPT_SBAS 2 /* ionosphere option: SBAS model */
#define IONOOPT_IFLC 3 /* ionosphere option: L1/L2 or L1/L5 iono-free LC */
#define IONOOPT_EST 4  /* ionosphere option: estimation */
#define IONOOPT_TEC 5  /* ionosphere option: IONEX TEC model */
#define IONOOPT_QZS 6  /* ionosphere option: QZSS broadcast model */
#define IONOOPT_LEX 7  /* ionosphere option: QZSS LEX ionospehre */
#define IONOOPT_STEC 8 /* ionosphere option: SLANT TEC model */

#define TROPOPT_OFF 0  /* troposphere option: correction off */
#define TROPOPT_SAAS 1 /* troposphere option: Saastamoinen model */
#define TROPOPT_SBAS 2 /* troposphere option: SBAS model */
#define TROPOPT_EST 3  /* troposphere option: ZTD estimation */
#define TROPOPT_ESTG 4 /* troposphere option: ZTD+grad estimation */
#define TROPOPT_ZTD 5  /* troposphere option: ZTD correction */

#define EPHOPT_BRDC 0   /* ephemeris option: broadcast ephemeris */
#define EPHOPT_PREC 1   /* ephemeris option: precise ephemeris */
#define EPHOPT_SBAS 2   /* ephemeris option: broadcast + SBAS */
#define EPHOPT_SSRAPC 3 /* ephemeris option: broadcast + SSR_APC */
#define EPHOPT_SSRCOM 4 /* ephemeris option: broadcast + SSR_COM */
#define EPHOPT_LEX 5    /* ephemeris option: QZSS LEX ephemeris */

#define ARMODE_OFF 0       /* AR mode: off */
#define ARMODE_CONT 1      /* AR mode: continuous */
#define ARMODE_INST 2      /* AR mode: instantaneous */
#define ARMODE_FIXHOLD 3   /* AR mode: fix and hold */
#define ARMODE_WLNL 4      /* AR mode: wide lane/narrow lane */
#define ARMODE_TCAR 5      /* AR mode: triple carrier ar */
#define ARMODE_PPPAR 6     /* AR mode: PPP-AR */
#define ARMODE_PPPAR_ILS 7 /* AR mode: PPP-AR ILS */

#define GLO_ARMODE_OFF 0                                /* GLO AR mode: off */
#define GLO_ARMODE_ON 1                                 /* GLO AR mode: on */
#define GLO_ARMODE_AUTOCAL 2 /* GLO AR mode: autocal */ //�Զ�У��
#define GLO_ARMODE_FIXHOLD 3                            /* GLO AR mode: fix and hold */

#define SBSOPT_LCORR 1 /* SBAS option: long term correction */
#define SBSOPT_FCORR 2 /* SBAS option: fast correction */
#define SBSOPT_ICORR 4 /* SBAS option: ionosphere correction */
#define SBSOPT_RANGE 8 /* SBAS option: ranging */

#define POSOPT_POS 0    /* pos option: LLH/XYZ */
#define POSOPT_SINGLE 1 /* pos option: average of single pos */
#define POSOPT_FILE 2   /* pos option: read from pos file */
#define POSOPT_RINEX 3  /* pos option: rinex header pos */
#define POSOPT_RTCM 4   /* pos option: rtcm station pos */
#define POSOPT_RAW 5    /* pos option: raw station pos */

#define STR_NONE 0      /* stream type: none */
#define STR_SERIAL 1    /* stream type: serial */
#define STR_FILE 2      /* stream type: file */
#define STR_TCPSVR 3    /* stream type: TCP server */
#define STR_TCPCLI 4    /* stream type: TCP client */
#define STR_NTRIPSVR 6  /* stream type: NTRIP server */
#define STR_NTRIPCLI 7  /* stream type: NTRIP client */
#define STR_FTP 8       /* stream type: ftp */
#define STR_HTTP 9      /* stream type: http */
#define STR_NTRIPC_S 10 /* stream type: NTRIP caster server */
#define STR_NTRIPC_C 11 /* stream type: NTRIP caster client */
#define STR_UDPSVR 12   /* stream type: UDP server */
#define STR_UDPCLI 13   /* stream type: UDP server */
#define STR_MEMBUF 14   /* stream type: memory buffer */

#define STRFMT_RTCM2 0   /* stream format: RTCM 2 */
#define STRFMT_RTCM3 1   /* stream format: RTCM 3 */
#define STRFMT_OEM4 2    /* stream format: NovAtel OEMV/4 */
#define STRFMT_CNAV 3    /* stream format: ComNav */
#define STRFMT_UBX 4     /* stream format: u-blox LEA-*T */
#define STRFMT_SBP 5     /* stream format: Swift Navigation SBP */
#define STRFMT_CRES 6    /* stream format: Hemisphere */
#define STRFMT_STQ 7     /* stream format: SkyTraq S1315F */
#define STRFMT_GW10 8    /* stream format: Furuno GW10 */
#define STRFMT_JAVAD 9   /* stream format: JAVAD GRIL/GREIS */
#define STRFMT_NVS 10    /* stream format: NVS NVC08C */
#define STRFMT_BINEX 11  /* stream format: BINEX */
#define STRFMT_RT17 12   /* stream format: Trimble RT17 */
#define STRFMT_SEPT 13   /* stream format: Septentrio */
#define STRFMT_CMR 14    /* stream format: CMR/CMR+ */
#define STRFMT_TERSUS 15 /* stream format: TERSUS */
#define STRFMT_LEXR 16   /* stream format: Furuno LPY-10000 */
#define STRFMT_RINEX 17  /* stream format: RINEX */
#define STRFMT_SP3 18    /* stream format: SP3 */
#define STRFMT_RNXCLK 19 /* stream format: RINEX CLK */
#define STRFMT_SBAS 20   /* stream format: SBAS messages */
#define STRFMT_NMEA 21   /* stream format: NMEA 0183 */
#ifndef EXTLEX
#define MAXRCVFMT 15 /* max number of receiver format */
#else
#define MAXRCVFMT 16
#endif

#define STR_MODE_R 0x1  /* stream mode: read */
#define STR_MODE_W 0x2  /* stream mode: write */
#define STR_MODE_RW 0x3 /* stream mode: read/write */

#define GEOID_EMBEDDED 0    /* geoid model: embedded geoid */
#define GEOID_EGM96_M150 1  /* geoid model: EGM96 15x15" */
#define GEOID_EGM2008_M25 2 /* geoid model: EGM2008 2.5x2.5" */
#define GEOID_EGM2008_M10 3 /* geoid model: EGM2008 1.0x1.0" */
#define GEOID_GSI2000_M15 4 /* geoid model: GSI geoid 2000 1.0x1.5" */
#define GEOID_RAF09 5       /* geoid model: IGN RAF09 for France 1.5"x2" */

#define COMMENTH "%"                   /* comment line indicator for solution */
#define MSG_DISCONN "$_DISCONNECT\r\n" /* disconnect message */

#define DLOPT_FORCE 0x01   /* download option: force download existing */
#define DLOPT_KEEPCMP 0x02 /* download option: keep compressed file */
#define DLOPT_HOLDERR 0x04 /* download option: hold on error file */
#define DLOPT_HOLDLST 0x08 /* download option: hold on listing file */

#define LLI_SLIP 0x01   /* LLI: cycle-slip */
#define LLI_HALFC 0x02  /* LLI: half-cycle not resovled */
#define LLI_BOCTRK 0x04 /* LLI: boc tracking of mboc signal */
#define LLI_HALFA 0x40  /* LLI: half-cycle added */
#define LLI_HALFS 0x80  /* LLI: half-cycle subtracted */

#define IMUFMT_KVH 1 /* imu data format KVH */

#define P2_5 0.03125                /* 2^-5 */
#define P2_6 0.015625               /* 2^-6 */
#define P2_11 4.882812500000000E-04 /* 2^-11 */
#define P2_15 3.051757812500000E-05 /* 2^-15 */
#define P2_17 7.629394531250000E-06 /* 2^-17 */
#define P2_19 1.907348632812500E-06 /* 2^-19 */
#define P2_20 9.536743164062500E-07 /* 2^-20 */
#define P2_21 4.768371582031250E-07 /* 2^-21 */
#define P2_23 1.192092895507810E-07 /* 2^-23 */
#define P2_24 5.960464477539063E-08 /* 2^-24 */
#define P2_27 7.450580596923828E-09 /* 2^-27 */
#define P2_29 1.862645149230957E-09 /* 2^-29 */
#define P2_30 9.313225746154785E-10 /* 2^-30 */
#define P2_31 4.656612873077393E-10 /* 2^-31 */
#define P2_32 2.328306436538696E-10 /* 2^-32 */
#define P2_33 1.164153218269348E-10 /* 2^-33 */
#define P2_35 2.910383045673370E-11 /* 2^-35 */
#define P2_38 3.637978807091710E-12 /* 2^-38 */
#define P2_39 1.818989403545856E-12 /* 2^-39 */
#define P2_40 9.094947017729280E-13 /* 2^-40 */
#define P2_43 1.136868377216160E-13 /* 2^-43 */
#define P2_48 3.552713678800501E-15 /* 2^-48 */
#define P2_50 8.881784197001252E-16 /* 2^-50 */
#define P2_55 2.775557561562891E-17 /* 2^-55 */

#ifdef WIN32
#define thread_t HANDLE
#define lock_t CRITICAL_SECTION
#define initlock(f) InitializeCriticalSection(f)
#define lock(f) EnterCriticalSection(f)
#define unlock(f) LeaveCriticalSection(f)
#define FILEPATHSEP '\\'
#define strnicmp(str1, str2, n) strnicmp(str1, str2, n)
#else
#define thread_t pthread_t
#define lock_t pthread_mutex_t
#define initlock(f) pthread_mutex_init(f, NULL)
#define lock(f) pthread_mutex_lock(f)
#define unlock(f) pthread_mutex_unlock(f)
#define FILEPATHSEP '/'
#define strnicmp(str1, str2, n) strncasecmp(str1, str2, n)
#endif

    /* type definitions ----------------------------------------------------------*/

#ifdef WIN32
    typedef struct
    {                /* time struct */
        time_t time; /* time (s) expressed by standard time_t */
        double sec;  /* fraction of second under 1 s */
    } gtime_t;
#else
typedef struct
{                                           /* time struct */
    time_t time;                            /* time (s) expressed by standard time_t */
    __attribute__((aligned(8))) double sec; /* fraction of second under 1 s */
} gtime_t;
#endif /*WIN32*/

    typedef struct
    {                                 /* observation data record */
        gtime_t time;                 /* receiver sampling time (GPST) */
        uint8_t sat, rcv;             /* satellite/receiver number */
        uint16_t SNR[NFREQ + NEXOBS]; /* signal strength (0.001 dBHz) */
        uint8_t LLI[NFREQ + NEXOBS];  /* loss of lock indicator */
        uint8_t code[NFREQ + NEXOBS]; /* code indicator (CODE_???) */
        double L[NFREQ + NEXOBS];     /* observation data carrier-phase (cycle) */
        double P[NFREQ + NEXOBS];     /* observation data pseudorange (m) */
        float D[NFREQ + NEXOBS];      /* observation data doppler frequency (Hz) */
    } obsd_t;

    typedef struct
    {                 /* observation data */
        int n, nmax;  /* number of obervation data/allocated */
        obsd_t *data; /* observation data records */
    } obs_t;

    typedef struct
    {                    /* earth rotation parameter data type */
        double mjd;      /* mjd (days) */
        double xp, yp;   /* pole offset (rad) */
        double xpr, ypr; /* pole offset rate (rad/day) */
        double ut1_utc;  /* ut1-utc (s) */
        double lod;      /* length of day (s/day) */
    } erpd_t;

    typedef struct
    {                 /* earth rotation parameter type */
        int n, nmax;  /* number and max number of data */
        erpd_t *data; /* earth rotation parameter data */
    } erp_t;

    typedef struct
    {                                  /* antenna parameter type */
        int sat;                       /* satellite number (0:receiver) */
        char type[MAXANT];             /* antenna type */
        char code[MAXANT];             /* serial number or satellite code */
        gtime_t ts, te;                /* valid time start and end */
        double off[NFREQ][3];          /* phase center offset e/n/u or x/y/z (m) */
        double var[NFREQ][19];         /* phase center variation (m) */
        double dazi;                   /* el=90,85,...,0 or nadir=0,1,2,3,... (deg) */
        double azivar[NFREQ][181][19]; // dazi=2
    } pcv_t;

    typedef struct
    {                /* antenna parameters type */
        int n, nmax; /* number of data/allocated */
        pcv_t *pcv;  /* antenna parameters data */
    } pcvs_t;

    typedef struct
    {                /* almanac type */
        int sat;     /* satellite number */
        int svh;     /* sv health (0:ok) */
        int svconf;  /* as and sv config */
        int week;    /* GPS/QZS: gps week, GAL: galileo week */
        gtime_t toa; /* Toa */
                     /* SV orbit parameters */
        double A, e, i0, OMG0, omg, M0, OMGd;
        double toas;   /* Toa (s) in week */
        double f0, f1; /* SV clock parameters (af0,af1) */
    } alm_t;

    typedef struct
    {                          /* GPS/QZS/GAL broadcast ephemeris type */
        int sat;               /* satellite number */
        int iode, iodc;        /* IODE,IODC */
        int sva;               /* SV accuracy (URA index) */
        int svh;               /* SV health (0:ok) */
        int week;              /* GPS/QZS: gps week, GAL: galileo week */
        int code;              /* GPS/QZS: code on L2, GAL/CMP: data sources */
        int flag;              /* GPS/QZS: L2 P data flag, CMP: nav type */
        gtime_t toe, toc, ttr; /* Toe,Toc,T_trans */
                               /* SV orbit parameters */
        double A, e, i0, OMG0, omg, M0, deln, OMGd, idot;
        double crc, crs, cuc, cus, cic, cis;
        double toes;       /* Toe (s) in week */
        double fit;        /* fit interval (h) */
        double f0, f1, f2; /* SV clock parameters (af0,af1,af2) */
        double tgd[4];     /* group delay parameters */
                           /* GPS/QZS:tgd[0]=TGD */
                           /* GAL    :tgd[0]=BGD E5a/E1,tgd[1]=BGD E5b/E1 */
                           /* CMP    :tgd[0]=BGD1,tgd[1]=BGD2 */
        double Adot, ndot; /* Adot,ndot for CNAV */
    } eph_t;

    typedef struct
    {                      /* GLONASS broadcast ephemeris type */
        int sat;           /* satellite number */
        int iode;          /* IODE (0-6 bit of tb field) */
        int frq;           /* satellite frequency number */
        int svh, sva, age; /* satellite health, accuracy, age of operation */
        gtime_t toe;       /* epoch of epherides (gpst) */
        gtime_t tof;       /* message frame time (gpst) */
        double pos[3];     /* satellite position (ecef) (m) */
        double vel[3];     /* satellite velocity (ecef) (m/s) */
        double acc[3];     /* satellite acceleration (ecef) (m/s^2) */
        double taun, gamn; /* SV clock bias (s)/relative freq bias */
        double dtaun;      /* delay between L1 and L2 (s) */
    } geph_t;

    typedef struct
    {                          /* precise ephemeris type */
        gtime_t time;          /* time (GPST) */
        int index;             /* ephemeris index for multiple files */
        double pos[MAXSAT][4]; /* satellite position/clock (ecef) (m|s) */
        float std[MAXSAT][4];  /* satellite position/clock std (m|s) */
        double vel[MAXSAT][4]; /* satellite velocity/clk-rate (m/s|s/s) */
        float vst[MAXSAT][4];  /* satellite velocity/clk-rate std (m/s|s/s) */
        float cov[MAXSAT][3];  /* satellite position covariance (m^2) */
        float vco[MAXSAT][3];  /* satellite velocity covariance (m^2) */
    } peph_t;

    typedef struct
    {                          /* precise clock type */
        gtime_t time;          /* time (GPST) */
        int index;             /* clock index for multiple files */
        double clk[MAXSAT][1]; /* satellite clock (s) */
        float std[MAXSAT][1];  /* satellite clock std (s) */
    } pclk_t;

    typedef struct
    {                    /* SBAS ephemeris type */
        int sat;         /* satellite number */
        gtime_t t0;      /* reference epoch time (GPST) */
        gtime_t tof;     /* time of message frame (GPST) */
        int sva;         /* SV accuracy (URA index) */
        int svh;         /* SV health (0:ok) */
        double pos[3];   /* satellite position (m) (ecef) */
        double vel[3];   /* satellite velocity (m/s) (ecef) */
        double acc[3];   /* satellite acceleration (m/s^2) (ecef) */
        double af0, af1; /* satellite clock-offset/drift (s,s/s) */
    } seph_t;            // ����ɾ��

    typedef struct
    {                   /* TEC grid type */
        gtime_t time;   /* epoch time (GPST) */
        int ndata[3];   /* TEC grid data size {nlat,nlon,nhgt} */
        double rb;      /* earth radius (km) */
        double lats[3]; /* latitude start/interval (deg) */
        double lons[3]; /* longitude start/interval (deg) */
        double hgts[3]; /* heights start/interval (km) */
        double *data;   /* TEC grid data (tecu) */
        float *rms;     /* RMS values (tecu) */
    } tec_t;

    typedef struct
    {                        /* satellite fcb data type */
        gtime_t ti;          /* time (GPST) */
        int iod;             /* AODC(s) */
        double bias[MAXSAT]; /* fcb value   (cyc) [3]->[0]*/
        double std[MAXSAT];  /* fcb std-dev (cyc) [3]->[0]*/
        int flag[MAXSAT];
    } fcbd_t;

    typedef struct
    {
        gtime_t ti;
        double bias[MAXSAT];
    } ifcb_t;

    typedef struct
    {                          /* SBAS message type */
        int week, tow;         /* receiption time */
        int prn;               /* SBAS satellite PRN number */
        unsigned char msg[29]; /* SBAS message (226bit) padded by 0 */
    } sbsmsg_t;                //ɾ��

    typedef struct
    {                   /* SBAS messages type */
        int n, nmax;    /* number of SBAS messages/allocated */
        sbsmsg_t *msgs; /* SBAS messages */
    } sbs_t;            //ɾ��

    typedef struct
    {               /* SBAS fast correction type */
        gtime_t t0; /* time of applicability (TOF) */
        double prc; /* pseudorange correction (PRC) (m) */
        double rrc; /* range-rate correction (RRC) (m/s) */
        double dt;  /* range-rate correction delta-time (s) */
        int iodf;   /* IODF (issue of date fast corr) */
        short udre; /* UDRE+1 */
        short ai;   /* degradation factor indicator */
    } sbsfcorr_t;

    typedef struct
    {                      /* SBAS long term satellite error correction type */
        gtime_t t0;        /* correction time */
        int iode;          /* IODE (issue of date ephemeris) */
        double dpos[3];    /* delta position (m) (ecef) */
        double dvel[3];    /* delta velocity (m/s) (ecef) */
        double daf0, daf1; /* delta clock-offset/drift (s,s/s) */
    } sbslcorr_t;

    typedef struct
    {                     /* SBAS satellite correction type */
        int sat;          /* satellite number */
        sbsfcorr_t fcorr; /* fast correction */
        sbslcorr_t lcorr; /* long term correction */
    } sbssatp_t;

    typedef struct
    {                          /* SBAS satellite corrections type */
        int iodp;              /* IODP (issue of date mask) */
        int nsat;              /* number of satellites */
        int tlat;              /* system latency (s) */
        sbssatp_t sat[MAXSAT]; /* satellite correction */
    } sbssat_t;

    typedef struct
    {                   /* SBAS ionospheric correction type */
        gtime_t t0;     /* correction time */
        short lat, lon; /* latitude/longitude (deg) */
        short give;     /* GIVI+1 */
        float delay;    /* vertical delay estimate (m) */
    } sbsigp_t;

    typedef struct
    {                       /* IGP band type */
        short x;            /* longitude/latitude (deg) */
        const short *y;     /* latitudes/longitudes (deg) */
        unsigned char bits; /* IGP mask start bit */
        unsigned char bite; /* IGP mask end bit */
    } sbsigpband_t;

    typedef struct
    {                          /* SBAS ionospheric corrections type */
        int iodi;              /* IODI (issue of date ionos corr) */
        int nigp;              /* number of igps */
        sbsigp_t igp[MAXNIGP]; /* ionospheric correction */
    } sbsion_t;

    typedef struct
    {                /* DGPS/GNSS correction type */
        gtime_t t0;  /* correction time */
        double prc;  /* pseudorange correction (PRC) (m) */
        double rrc;  /* range rate correction (RRC) (m/s) */
        int iod;     /* issue of data (IOD) */
        double udre; /* UDRE */
    } dgps_t;

    typedef struct
    {                             /* SSR correction type */
        gtime_t t0[6];            /* epoch time (GPST) {eph,clk,hrclk,ura,bias,pbias} */
        double udi[6];            /* SSR update interval (s) */
        int iod[6];               /* iod ssr {eph,clk,hrclk,ura,bias,pbias} */
        int iode;                 /* issue of data */
        int iodcrc;               /* issue of data crc for beidou/sbas */
        int ura;                  /* URA indicator */
        int refd;                 /* sat ref datum (0:ITRF,1:regional) */
        double deph[3];           /* delta orbit {radial,along,cross} (m) */
        double ddeph[3];          /* dot delta orbit {radial,along,cross} (m/s) */
        double dclk[3];           /* delta clock {c0,c1,c2} (m,m/s,m/s^2) */
        double hrclk;             /* high-rate clock corection (m) */
        float cbias[MAXCODE];     /* code biases (m) */
        double pbias[MAXCODE];    /* phase biases (m) */
        float stdpb[MAXCODE];     /* std-dev of phase biases (m) */
        double yaw_ang, yaw_rate; /* yaw angle and yaw rate (deg,deg/s) */
        unsigned char update;     /* update flag (0:no update,1:update) */
    } ssr_t;

    typedef struct
    {                           /* QZSS LEX message type */
        int prn;                /* satellite PRN number */
        int type;               /* message type */
        int alert;              /* alert flag */
        unsigned char stat;     /* signal tracking status */
        unsigned char snr;      /* signal C/N0 (0.25 dBHz) */
        unsigned int ttt;       /* tracking time (ms) */
        unsigned char msg[212]; /* LEX message data part 1695 bits */
    } lexmsg_t;

    typedef struct
    {                   /* QZSS LEX messages type */
        int n, nmax;    /* number of LEX messages and allocated */
        lexmsg_t *msgs; /* LEX messages */
    } lex_t;

    typedef struct
    {                         /* QZSS LEX ephemeris type */
        gtime_t toe;          /* epoch time (GPST) */
        gtime_t tof;          /* message frame time (GPST) */
        int sat;              /* satellite number */
        unsigned char health; /* signal health (L1,L2,L1C,L5,LEX) */
        unsigned char ura;    /* URA index */
        double pos[3];        /* satellite position (m) */
        double vel[3];        /* satellite velocity (m/s) */
        double acc[3];        /* satellite acceleration (m/s2) */
        double jerk[3];       /* satellite jerk (m/s3) */
        double af0, af1;      /* satellite clock bias and drift (s,s/s) */
        double tgd;           /* TGD */
        double isc[8];        /* ISC */
    } lexeph_t;

    typedef struct
    {                      /* QZSS LEX ionosphere correction type */
        gtime_t t0;        /* epoch time (GPST) */
        double tspan;      /* valid time span (s) */
        double pos0[2];    /* reference position {lat,lon} (rad) */
        double coef[3][2]; /* coefficients lat x lon (3 x 2) */
    } lexion_t;

#define MAXTIM 2880 * 2
    typedef struct
    {                  /* trop data type */
        gtime_t time;  /* time (GPST) */
        double trp[3]; /* zenith tropos delay/gradient (m) */
        double std[3]; /* std-dev (m) */
    } trop_t;
    typedef struct
    {                  /* ppp corrections type */
        char stas[8];  /* station names */
        double rr[3];  /* station ecef positions (m) */
        int nt, ntmax; /* number of trop data */
        trop_t *data;  /* trop data */
    } corrtrop_t;

    typedef struct
    {                /* stec data type */
        uint8_t sat; /* satellite number */
        double ion;  /* slant ionos delay (m) */
        float std;   /* std-dev (m) */
        float el;
        int lock1;
    } stec_t;
    typedef struct
    {                         /* ppp corrections type */
        char stas[8];         /* station names */
        double rr[3];         /* station ecef positions (m) */
        int nn, ii[MAXTIM];   /* number of stec data */
        gtime_t time[MAXTIM]; /* time list (GPST) */
        int ns, nsmax;
        stec_t *data; /* stec data */
    } corrstec_t;

    typedef struct
    {                      /* navigation data type */
        int n, nmax;       /* number of broadcast ephemeris */
        int ng, ngmax;     /* number of glonass ephemeris */
        int ns, nsmax;     /* number of sbas ephemeris */
        int ne, nemax;     /* number of precise ephemeris */
        int nc, ncmax;     /* number of precise clock */
        int na, namax;     /* number of almanac data */
        int nt, ntmax;     /* number of tec grid data */
        int nf, nfmax;     /* number of satellite fcb data */
        int ni, nimax;     /* number of satellite ifcb data */
        eph_t *eph;        /* GPS/QZS/GAL ephemeris */
        geph_t *geph;      /* GLONASS ephemeris */
        seph_t *seph;      /* SBAS ephemeris */
        peph_t *peph;      /* precise ephemeris */
        pclk_t *pclk;      /* precise clock */
        alm_t *alm;        /* almanac data */
        tec_t *tec;        /* tec grid data */
        fcbd_t *fcb;       /* satellite fcb data */
        ifcb_t *ifcb;      /* satellite ifcb data */
        erp_t erp;         /* earth rotation parameters */
        double utc_gps[4]; /* GPS delta-UTC parameters {A0,A1,T,W} */
        double utc_glo[4]; /* GLONASS UTC GPS time parameters */
        double utc_gal[4]; /* Galileo UTC GPS time parameters */
        double utc_qzs[4]; /* QZS UTC GPS time parameters */
        double utc_cmp[4]; /* BeiDou UTC parameters */
        double utc_irn[4]; /* IRNSS UTC parameters */
        double utc_sbs[4]; /* SBAS UTC parameters */
        double ion_gps[8]; /* GPS iono model parameters {a0,a1,a2,a3,b0,b1,b2,b3} */
        double ion_gal[4]; /* Galileo iono model parameters {ai0,ai1,ai2,0} */
        double ion_qzs[8]; /* QZSS iono model parameters {a0,a1,a2,a3,b0,b1,b2,b3} */
        double ion_cmp[8]; /* BeiDou iono model parameters {a0,a1,a2,a3,b0,b1,b2,b3} */
        double ion_irn[8]; /* IRNSS iono model parameters {a0,a1,a2,a3,b0,b1,b2,b3} */
        // int leaps;          /* leap seconds (s) */
        double cbias[MAXSAT][3];    /* satellite dcb (0:p1-p2,1:p1-c1,2:p2-c2) (m) */
        double rbias[MAXRCV][2][3]; /* receiver dcb (0:p1-p2,1:p1-c1,2:p2-c2) (m) */
        double wlbias[MAXSAT];      /* wide-lane bias (cycle) */
        double elbias[MAXSAT];      /* wide-lane bias (cycle) */
        // double glo_cpbias[4];    /* glonass code-phase bias {1C,1P,2C,2P} (m) */
        char glo_fcn[MAXPRNGLO + 1]; /* glonass frequency channel number + 8 */
        pcv_t pcvs[MAXSAT];          /* satellite antenna pcv */
        sbssat_t sbssat;             /* SBAS satellite corrections */
        ssr_t ssr[MAXSAT];           /* SSR corrections */
        int nstec, ntrop;
        corrtrop_t corrtrop[25];
        corrstec_t corrstec[25];
    } nav_t;

    typedef struct
    {                          /* station parameter type */
        char name[MAXANT];     /* marker name */
        char marker[MAXANT];   /* marker number */
        char antdes[MAXANT];   /* antenna descriptor */
        char antsno[MAXANT];   /* antenna serial number */
        char rectype[MAXANT];  /* receiver type descriptor */
        char recver[MAXANT];   /* receiver firmware version */
        char recsno[MAXANT];   /* receiver serial number */
        int antsetup;          /* antenna setup id */
        int itrf;              /* ITRF realization year */
        int deltype;           /* antenna delta type (0:enu,1:xyz) */
        double pos[3];         /* station position (ecef) (m) */
        double del[3];         /* antenna position delta (e/n/u or x/y/z) (m) */
        double hgt;            /* antenna height (m) */
        int glo_cp_align;      /* GLONASS code-phase alignment (0:no,1:yes) */
        double glo_cp_bias[4]; /* GLONASS code-phase biases {1C,1P,2C,2P} (m) */
    } sta_t;
    typedef struct
    {
        int n, nmax; /* number of data/allocated */
        sta_t *data; /* station parameters data */
    } stas_t;
    typedef struct
    {                      /* solution type */
        gtime_t time;      /* time (GPST) */
        double rr[6];      /* position/velocity (m|m/s) */
        double pr[6];      /* precious position */
                           /* {x,y,z,vx,vy,vz} or {e,n,u,ve,vn,vu} */
        float qr[6];       /* position variance/covariance (m^2) */
                           /* {c_xx,c_yy,c_zz,c_xy,c_yz,c_zx} or */
                           /* {c_ee,c_nn,c_uu,c_en,c_nu,c_ue} */
        float qv[6];       /* velocity variance/covariance (m^2/s^2) */
        double dtr[6];     /* receiver clock bias to time systems (s) 6>12*/
        uint8_t type;      /* type (0:xyz-ecef,1:enu-baseline) */
        uint8_t stat;      /* solution status (SOLQ_???) */
        uint8_t ns;        /* number of valid satellites */
        float age;         /* age of differential (s) */
        float ratio;       /* AR ratio factor for valiation */
        float prev_ratio1; /* previous initial AR ratio factor for validation */
        float prev_ratio2; /* previous final AR ratio factor for validation */
        float thres;       /* AR ratio threshold for valiation */
        uint8_t nsat;      /* 2022-08-17 add by zh  plot nsat */
        double pdop;       /* 2022-08-17 add by zh  plot pdop */
    } sol_t;

    typedef struct
    {                                      /* solution buffer type */
        int n, nmax;                       /* number of solution/max number of buffer */
        int cyclic;                        /* cyclic buffer flag */
        int start, end;                    /* start/end index */
        gtime_t time;                      /* current solution time */
        sol_t *data;                       /* solution data */
        double rb[3];                      /* reference position {x,y,z} (ecef) (m) */
        unsigned char buff[MAXSOLMSG + 1]; /* message buffer */
        int nb;                            /* number of byte in message buffer */
    } solbuf_t;

    typedef struct
    {                         /* solution status type */
        gtime_t time;         /* time (GPST) */
        unsigned char sat;    /* satellite number */
        unsigned char frq;    /* frequency (1:L1,2:L2,...) */
        float az, el;         /* azimuth/elevation angle (rad) */
        float resp;           /* pseudorange residual (m) */
        float resc;           /* carrier-phase residual (m) */
        unsigned char flag;   /* flags: (vsat<<5)+(slip<<3)+fix */
        unsigned char snr;    /* signal strength (0.25 dBHz) */
        unsigned short lock;  /* lock counter */
        unsigned short outc;  /* outage counter */
        unsigned short slipc; /* slip counter */
        unsigned short rejc;  /* reject counter */
    } solstat_t;

    typedef struct
    {                    /* solution status buffer type */
        int n, nmax;     /* number of solution/max number of buffer */
        solstat_t *data; /* solution status data */
    } solstatbuf_t;

    typedef struct
    {                                                /* RTCM control struct type */
        int staid;                                   /* station id */
        int stah;                                    /* station health */
        int seqno;                                   /* sequence number for rtcm 2 or iods msm */
        int outtype;                                 /* output message type */
        gtime_t time;                                /* message time */
        gtime_t time_s;                              /* message start time */
        obs_t obs;                                   /* observation data (uncorrected) */
        nav_t nav;                                   /* satellite ephemerides */
        sta_t sta;                                   /* station parameters */
        dgps_t *dgps;                                /* output of dgps corrections */
        ssr_t ssr[MAXSAT];                           /* output of ssr corrections */
        char msg[128];                               /* special message */
        char msgtype[256];                           /* last message type */
        char msmtype[6][128];                        /* msm signal types */
        int obsflag;                                 /* obs data complete flag (1:ok,0:not complete) */
        int ephsat;                                  /* update satellite of ephemeris */
        double cp[MAXSAT][NFREQ + NEXOBS];           /* carrier-phase measurement */
        unsigned short lock[MAXSAT][NFREQ + NEXOBS]; /* lock time */
        unsigned short loss[MAXSAT][NFREQ + NEXOBS]; /* loss of lock count */
        gtime_t lltime[MAXSAT][NFREQ + NEXOBS];      /* last lock time */
        int nbyte;                                   /* number of bytes in message buffer */
        int nbit;                                    /* number of bits in word buffer */
        int len;                                     /* message length (bytes) */
        unsigned char buff[1200];                    /* message buffer */
        unsigned int word;                           /* word buffer for rtcm 2 */
        unsigned int nmsg2[100];                     /* message count of RTCM 2 (1-99:1-99,0:other) */
        unsigned int nmsg3[400]; /* message count of RTCM 3 (1-299:1001-1299,300-399:2000-2099,0:ohter) */
        char opt[256];           /* RTCM dependent options */
    } rtcm_t;

    typedef struct
    {                                /* rinex control struct type */
        gtime_t time;                /* message time */
        double ver;                  /* rinex version */
        char type;                   /* rinex file type ('O','N',...) */
        int sys;                     /* navigation system */
        int tsys;                    /* time system */
        char tobs[7][MAXOBSTYPE][4]; /* rinex obs types */
        obs_t obs;                   /* observation data */
        nav_t nav;                   /* navigation data */
        sta_t sta;                   /* station info */
        int ephsat;                  /* ephemeris satellite number */
        int ephset;                  /* input ephemeris set (0-1) */
        char opt[256];               /* rinex dependent options */
    } rnxctr_t;

    typedef struct
    {                    /* download url type */
        char type[32];   /* data type */
        char path[1024]; /* url path */
        char dir[1024];  /* local directory */
        double tint;     /* time interval (s) */
    } url_t;

    typedef struct
    {                        /* option type */
        const char *name;    /* option name */
        int format;          /* option format (0:int,1:double,2:string,3:enum) */
        void *var;           /* pointer to option variable */
        const char *comment; /* option comment/enum labels/unit */
    } opt_t;

    typedef struct
    {                              /* extended receiver error model */
        int ena[4];                /* model enabled */
        double cerr[4][NFREQ * 2]; /* code errors (m) */
        double perr[4][NFREQ * 2]; /* carrier-phase errors (m) */
    } exterr_t;

    typedef struct
    {                          /* SNR mask type */
        int ena[2];            /* enable flag {rover,base} */
        double mask[NFREQ][9]; /* mask (dBHz) at 5,10,...85 deg */
    } snrmask_t;

    typedef struct
    {                            /* processing options type */
        int mode;                /* positioning mode (PMODE_???) */
        gtime_t ts, te;          /* process time */
        double ti, reinit;       /* process time interval | reinit state time*/
        int soltype;             /* solution type (0:forward,1:backward,2:combined) */
        int nf;                  /* number of frequencies (1:L1,2:L1+L2,3:L1+L2+L5) */
        int navsys;              /* navigation system */
        double elmin;            /* elevation mask angle (rad) */
        snrmask_t snrmask;       /* SNR mask */
        int sateph;              /* satellite ephemeris/clock (EPHOPT_???) */
        int modear;              /* AR mode (0:off,1:continuous,2:instantaneous,3:fix-and-hold,6:ppp-ar,7:ppp-ar-ils) */
        int typear;              /* AR type (0:overall-ar,1:part-ar1,2:part-ar2) */
        int maxout[4];           /* obs outage count to reset [0], [1]iono,[2]ifb,[3]bias  */
        int maxrej[4];           /* obs reject count to reset [0], [1]iono,[2]ifb,[3]bias */
        int acctime;             /* accept max diff time (sec) */
        int minlock;             /* min lock count to fix ambiguity */
        int minfixsats;          /* min sats to fix integer ambiguities */
        int ionoopt;             /* ionosphere option (IONOOPT_???) */
        int tropopt;             /* troposphere option (TROPOPT_???) */
        int dynamics;            /* dynamics model (0:none,1:accel,2:sins) */
        int tidecorr;            /* earth tide correction (0:off,1:solid,2:solid+otl+pole) */
        int codesmooth;          /* code smoothing window size (0:none) */
        int intpref;             /* interpolate reference obs (for post mission) */
        int rovpos;              /* rover position for fixed mode */
        int refpos;              /* base position for relative mode */
                                 /* (0:pos in prcopt,  1:average of single pos, */
                                 /*  2:read from file, 3:rinex header, 4:rtcm pos) */
        int outopt;              /* 1:status+2:iono+4:ambi+8:resi+16:qual  */
        double eratio[NFREQ];    /* code/phase error ratio */
        double err[7];           /* measurement error factor */
                                 /* [0]:reserved */
                                 /* [1-3]:error factor a/b/c of phase (m) */
                                 /* [4]:doppler frequency (hz) */
                                 /* [5-6]: exterior trop iono */
        double noise[3];         /* noise std [0]:dcb,[1]glo-ifb */
        double P0[8];            /* initial-state std [0]pos,[1]clk,[2]trop,[3]iono,[4]ifb,[5]bias,[6]vel, [7]acc */
        double Qt[8];            /* process-noise std [0]pos,[1]   ,[2]trop,[3]iono,[4]ifb,[5]bias,[6]acch,[7]accv*/
        double sclkstab;         /* satellite clock stability (sec/sec) */
        double thresar[8];       /* AR validation threshold 0:ewl 1:wl 2:nl 3:ratio */
        double elmaskar;         /* elevation mask of AR for rising satellite (rad) */
        double thresslipgf[3];   /* slip threshold of geometry-free phase (m) 0:L1L2 1:L2L3 2: */
        double thresslipmw[3];   /* slip threshold of MW phase (cycle) 0:L1L2 1:L2L3 2: */
        double maxtdiff;         /* max difference of time (sec) */
        double maxgdop;          /* reject threshold of gdop */
        double threscheck[6];    /* check  threshold 0:pri 1:pro 2:tdcp 3:p1p2 4:spp 5:ppp-ar */
        double baseline[2];      /* baseline length constraint {const,sigma} (m) */
        double ru[3];            /* rover position for fixed mode {x,y,z} (ecef) (m) */
        double rb[3];            /* base position for relative mode {x,y,z} (ecef) (m) */
        char anttype[2][MAXANT]; /* antenna types {rover,base} */
        double antdel[2][3];     /* antenna delta {{rov_e,rov_n,rov_u},{ref_e,ref_n,ref_u}} */
        pcv_t pcvr[2];           /* receiver antenna parameters {rov,base} */
        unsigned char exsats[MAXSAT]; /* excluded satellites (1:excluded,2:included) */
        char rnxopt[2][256];          /* rinex options {rover,base} */
        int posopt[9];                /* positioning options */
        double odisp[2][6 * 11];      /* ocean tide loading parameters {rov,base} */
        exterr_t exterr;              /* extended receiver error model */
    } prcopt_t;

    typedef struct
    {                       /* solution options type */
        int posf;           /* solution format (SOLF_???) */
        int times;          /* time system (TIMES_???) */
        int timef;          /* time format (0:sssss.s,1:yyyy/mm/dd hh:mm:ss.s) */
        int timeu;          /* time digits under decimal point */
        int degf;           /* latitude/longitude format (0:ddd.ddd,1:ddd mm ss) */
        int outhead;        /* output header (0:no,1:yes) */
        int outopt;         /* output processing options (0:no,1:yes) */
        int outvel;         /* output velocity options (0:no,1:yes) */
        int datum;          /* datum (0:WGS84,1:Tokyo) */
        int height;         /* height (0:ellipsoidal,1:geodetic) */
        int geoid;          /* geoid model (0:EGM96,1:JGD2000) */
        int solstatic;      /* solution of static mode (0:all,1:single) */
        int sstat;          /* solution statistics level (0:off,1:states,2:residuals) */
        int trace;          /* debug trace level (0:off,1-5:debug) */
        double nmeaintv[2]; /* nmea output interval (s) (<0:no,0:all) */
                            /* nmeaintv[0]:gprmc,gpgga,nmeaintv[1]:gpgsv */
        char sep[64];       /* field separator */
        char prog[64];      /* program name */
        double maxsolstd;   /* max std-dev for solution output (m) (0:all) */
    } solopt_t;

    typedef struct
    {                             /* file options type */
        char satantp[MAXSTRPATH]; /* satellite antenna parameters file */
        char rcvantp[MAXSTRPATH]; /* receiver antenna parameters file */
        char stapos[MAXSTRPATH];  /* station positions file */
        char geoid[MAXSTRPATH];   /* external geoid data file */
        char iono[MAXSTRPATH];    /* ionosphere data file */
        char dcb[MAXSTRPATH];     /* dcb data file */
        char eop[MAXSTRPATH];     /* eop data file */
        char blq[MAXSTRPATH];     /* ocean tide loading blq file */
        char tempdir[MAXSTRPATH]; /* ftp/http temporaly directory */
        char geexe[MAXSTRPATH];   /* google earth exec file(这里是kml输出路径) */
        char solstat[MAXSTRPATH]; /* solution statistics file */
        char trace[MAXSTRPATH];   /* debug trace file */
        char clk[MAXSTRPATH];     /* precise clock */
        char sp3[MAXSTRPATH];     /* precise orbit */
        char fcb[MAXSTRPATH];     /* fcb data file */
        char ifcb[MAXSTRPATH];    /* ifcb data file */
        char brdc[MAXSTRPATH];    /* brdc file */
        char stas[MAXSTRPATH];    /* stas file */
        char corr[MAXSTRPATH];    /* corr file */
        char rovobs[MAXSTRPATH];
        char refobs[MAXSTRPATH];
        char outdir[MAXSTRPATH];
        char outfile1[MAXSTRPATH];
        char outfile2[MAXSTRPATH];
    } filopt_t;

    typedef struct
    {                                 /* RINEX options type */
        gtime_t ts, te;               /* time start/end */
        double tint;                  /* time interval (s) */
        double ttol;                  /* time tolerance (s) */
        double tunit;                 /* time unit for multiple-session (s) */
        double rnxver;                /* RINEX version */
        int navsys;                   /* navigation system */
        int obstype;                  /* observation type */
        int freqtype;                 /* frequency type */
        char mask[7][64];             /* code mask {GPS,GLO,GAL,QZS,SBS,CMP,IRN} */
        char staid[32];               /* station id for rinex file name */
        char prog[32];                /* program */
        char runby[32];               /* run-by */
        char marker[64];              /* marker name */
        char markerno[32];            /* marker number */
        char markertype[32];          /* marker type (ver.3) */
        char name[2][32];             /* observer/agency */
        char rec[3][32];              /* receiver #/type/vers */
        char ant[3][32];              /* antenna #/type */
        double apppos[3];             /* approx position x/y/z */
        double antdel[3];             /* antenna delta h/e/n */
        double glo_cp_bias[4];        /* GLONASS code-phase biases (m) */
        char comment[MAXCOMMENT][64]; /* comments */
        char rcvopt[256];             /* receiver dependent options */
        unsigned char exsats[MAXSAT]; /* excluded satellites */
        int scanobs;                  /* scan obs types */
        int outiono;                  /* output iono correction */
        int outtime;                  /* output time system correction */
        int outleaps;                 /* output leap seconds */
        int autopos;                  /* auto approx position */
        int halfcyc;                  /* half cycle correction */
        int sep_nav;                  /* separated nav files */
        gtime_t tstart;               /* first obs time */
        gtime_t tend;                 /* last obs time */
        gtime_t trtcm;                /* approx log start time for rtcm */
        char tobs[7][MAXOBSTYPE][4];  /* obs types {GPS,GLO,GAL,QZS,SBS,CMP,IRN} */
        double shift[7][MAXOBSTYPE];  /* phase shift (cyc) {GPS,GLO,GAL,QZS,SBS,CMP,IRN} */
        int nobs[7];                  /* number of obs types {GPS,GLO,GAL,QZS,SBS,CMP,IRN} */
    } rnxopt_t;

    typedef struct
    {                        /* satellite status type */
        uint8_t sys;         /* navigation system */
        uint8_t vs;          /* valid satellite flag single */
        uint8_t vsat[NFREQ]; /* valid process flag 1:vaildP 2:vaildL */
        uint8_t eobs;        /* exclude obs flag 1-3:pseduo 5-7:L */
        uint8_t ini;         /* init state flag 1:iono 2:ifb 16:iniAMB1 32:iniAMB2 64:iniAMB3 */
        uint8_t reset;       /* reset 1:iono 2:ifb 5:amb1 6:amb2 7:amb3 */
        uint8_t fix[NFREQ];  /* ambiguity fix flag (2:fix,1:float,3:hold) */
        uint8_t rejp[NFREQ]; /* reject  pseudorange  cout */
        uint8_t rejc[NFREQ]; /* reject carrier-phase cout */
        uint8_t rejw;
        uint8_t slip[NFREQ];  /* cycle-slip flag */
        uint8_t half[NFREQ];  /* half-cycle valid flag */
        uint32_t lock[NFREQ]; /* lock counter of phase */
        uint32_t outc[NFREQ]; /* obs outage counter of phase */

        double azel[2];     /* azimuth/elevation angles {az,el} (rad) */
        double resp[NFREQ]; /* residuals of pseudorange (m) */
        double resc[NFREQ]; /* residuals of carrier-phase (m) */
        double respp;
        double phw;    /* phase windup (cycle) */
        double php;    /* phase prealign (m) */
        double LC[10]; /* linear combination 0:GF12 1:GF23 2:GF13 3:MW-L12P12 4:MW-L23P12 5:MW-L13P12 6:MP1 7:MP2 8:MP3
                          9:DIF */

        double diff[13]; /* diff ambiguity 0-5:dGF12 dGF23 dGF13 dMW12 dMW23 dMW13  6-8:TDCP   9-11:p1p2 p2p3 p1p3
                            12:dp1+ddopper */

        gtime_t prev_T[2]; /* previous pseudorange & carrier-phase & Dopple 0:rover 1:base */
        double prev_P[2][NFREQ];
        double prev_L[2][NFREQ];
        double prev_D[2][NFREQ];
        double prev_LC[10]; /* previous linear combination */
        double prev_rs[6];
        double prev_dts[2];
    } ssat_t;

    typedef struct
    {                                                            /* RTK control/result type */
        sol_t sol;                                               /* RTK solution */
        double rb[6];                                            /* base position/velocity (ecef) (m|m/s) */
        int nx, na;                                              /* number of float states/fixed states */
        double tt;                                               /* time difference between current and previous (s) */
        double *x, *P; /* float states and their covariance */   // nx
        double *xa, *Pa; /* fixed states and their covariance */ // na
        int nfix;                                                /* number of continuous fixes of ambiguity */
        ssat_t ssat[MAXSAT];                                     /* satellite status */
        int neb;                                                 /* bytes in error message buffer */
        char errbuf[MAXERRMSG];                                  /* error message buffer */
        prcopt_t opt;                                            /* processing options */
        double dr[3];                                            /* earth tides correction */
        uint8_t reset;                                           /* reset pos & vel & acc & trp flag */
    } rtk_t;

    typedef struct
    {                                           /* receiver raw data control type */
        gtime_t time;                           /* message time */
        gtime_t tobs[MAXSAT][NFREQ + NEXOBS];   /* observation data time */
        obs_t obs;                              /* observation data */
        obs_t obuf;                             /* observation data buffer */
        nav_t nav;                              /* satellite ephemerides */
        sta_t sta;                              /* station parameters */
        int ephsat;                             /* sat number of update ephemeris (0:no satellite) */
        sbsmsg_t sbsmsg;                        /* SBAS message */
        char msgtype[256];                      /* last message type */
        uint8_t subfrm[MAXSAT][380];            /* subframe buffer */
        lexmsg_t lexmsg;                        /* LEX message */
        double lockt[MAXSAT][NFREQ + NEXOBS];   /* lock time (s) */
        double icpp[MAXSAT], off[MAXSAT], icpc; /* carrier params for ss2 */
        double prCA[MAXSAT], dpCA[MAXSAT];      /* L1/CA pseudrange/doppler for javad */
        uint8_t halfc[MAXSAT][NFREQ + NEXOBS];  /* half-cycle add flag */
        char freqn[MAXOBS];                     /* frequency number for javad */
        int nbyte;                              /* number of bytes in message buffer */
        int len;                                /* message length (bytes) */
        int iod;                                /* issue of data */
        int tod;                                /* time of day (ms) */
        int tbase;                              /* time base (0:gpst,1:utc(usno),2:glonass,3:utc(su) */
        int flag;                               /* general purpose flag */
        int outtype;                            /* output message type */
        uint8_t buff[MAXRAWLEN];                /* message buffer */
        char opt[256];                          /* receiver dependent options */

        int format;     /* receiver stream format */
        void *rcv_data; /* receiver dependent data */
    } raw_t;

    typedef struct
    {                             /* stream type */
        int type;                 /* type (STR_???) */
        int mode;                 /* mode (STR_MODE_?) */
        int state;                /* state (-1:error,0:close,1:open) */
        unsigned int inb, inr;    /* input bytes/rate */
        unsigned int outb, outr;  /* output bytes/rate */
        unsigned int tick_i;      /* input tick tick */
        unsigned int tick_o;      /* output tick */
        unsigned int tact;        /* active tick */
        unsigned int inbt, outbt; /* input/output bytes at tick */
        lock_t lock;              /* lock flag */
        void *port;               /* type dependent port control struct */
        char path[MAXSTRPATH];    /* stream path */
        char msg[MAXSTRMSG];      /* stream message */
    } stream_t;

    typedef struct
    {                          /* stream converter type */
        int itype, otype;      /* input and output stream type */
        int nmsg;              /* number of output messages */
        int msgs[32];          /* output message types */
        double tint[32];       /* output message intervals (s) */
        unsigned int tick[32]; /* cycle tick of output message */
        int ephsat[32];        /* satellites of output ephemeris */
        int stasel;            /* station info selection (0:remote,1:local) */
        rtcm_t rtcm;           /* rtcm input data buffer */
        raw_t raw;             /* raw  input data buffer */
        rtcm_t out;            /* rtcm output data buffer */
    } strconv_t;

    typedef struct
    {                                      /* stream server type */
        int state;                         /* server state (0:stop,1:running) */
        int cycle;                         /* server cycle (ms) */
        int buffsize;                      /* input/monitor buffer size (bytes) */
        int nmeacycle;                     /* NMEA request cycle (ms) (0:no) */
        int relayback;                     /* relay back of output streams (0:no) */
        int nstr;                          /* number of streams (1 input + (nstr-1) outputs */
        int npb;                           /* data length in peek buffer (bytes) */
        char cmds_periodic[16][MAXRCVCMD]; /* periodic commands */
        double nmeapos[3];                 /* NMEA request position (ecef) (m) */
        unsigned char *buff;               /* input buffers */
        unsigned char *pbuf;               /* peek buffer */
        unsigned int tick;                 /* start tick */
        stream_t stream[16];               /* input/output streams */
        strconv_t *conv[16];               /* stream converter */
        thread_t thread;                   /* server thread */
        lock_t lock;                       /* lock flag */
    } strsvr_t;

    typedef struct
    {                                     /* RTK server type */
        int state;                        /* server state (0:stop,1:running) */
        int cycle;                        /* processing cycle (ms) */
        int nmeacycle;                    /* NMEA request cycle (ms) (0:no req) */
        int nmeareq;                      /* NMEA request (0:no,1:nmeapos,2:single sol) */
        double nmeapos[3];                /* NMEA request position (ecef) (m) */
        int buffsize;                     /* input buffer size (bytes) */
        int format[3];                    /* input format {rov,base,corr} */
        solopt_t solopt[2];               /* output solution options {sol1,sol2} */
        int navsel;                       /* ephemeris select (0:all,1:rover,2:base,3:corr) */
        int nsbs;                         /* number of sbas message */
        int nsol;                         /* number of solution buffer */
        rtk_t rtk;                        /* RTK control/result struct */
        int nb[3];                        /* bytes in input buffers {rov,base} */
        int nsb[2];                       /* bytes in soulution buffers */
        int npb[3];                       /* bytes in input peek buffers */
        unsigned char *buff[3];           /* input buffers {rov,base,corr} */
        unsigned char *sbuf[2];           /* output buffers {sol1,sol2} */
        unsigned char *pbuf[3];           /* peek buffers {rov,base,corr} */
        sol_t solbuf[MAXSOLBUF];          /* solution buffer */
        unsigned int nmsg[3][10];         /* input message counts */
        raw_t raw[3];                     /* receiver raw control {rov,base,corr} */
        rtcm_t rtcm[3];                   /* RTCM control {rov,base,corr} */
        gtime_t ftime[3];                 /* download time {rov,base,corr} */
        char files[3][MAXSTRPATH];        /* download paths {rov,base,corr} */
        obs_t obs[3][MAXOBSBUF];          /* observation data {rov,base,corr} */
        nav_t nav;                        /* navigation data */
        sbsmsg_t sbsmsg[MAXSBSMSG];       /* SBAS message buffer */
        stream_t stream[8];               /* streams {rov,base,corr,sol1,sol2,logr,logb,logc} */
        stream_t *moni;                   /* monitor stream */
        unsigned int tick;                /* start tick */
        thread_t thread;                  /* server thread */
        int cputime;                      /* CPU time (ms) for a processing cycle */
        int prcout;                       /* missing observation data count */
        int nave;                         /* number of averaging base pos */
        double rb_ave[3];                 /* averaging base pos */
        char cmds_periodic[3][MAXRCVCMD]; /* periodic commands */
        char cmd_reset[MAXRCVCMD];        /* reset command */
        double bl_reset;                  /* baseline length to reset (km) */
        lock_t lock;                      /* lock flag */
    } rtksvr_t;

    typedef struct
    {                  /* gis data point type */
        double pos[3]; /* point data {lat,lon,height} (rad,m) */
    } gis_pnt_t;

    typedef struct
    {                    /* gis data polyline type */
        int npnt;        /* number of points */
        double bound[4]; /* boundary {lat0,lat1,lon0,lon1} */
        double *pos;     /* position data (3 x npnt) */
    } gis_poly_t;

    typedef struct
    {                    /* gis data polygon type */
        int npnt;        /* number of points */
        double bound[4]; /* boundary {lat0,lat1,lon0,lon1} */
        double *pos;     /* position data (3 x npnt) */
    } gis_polygon_t;

    typedef struct gisd_tag
    {                          /* gis data list type */
        int type;              /* data type (1:point,2:polyline,3:polygon) */
        void *data;            /* data body */
        struct gisd_tag *next; /* pointer to next */
    } gisd_t;

    typedef struct
    {                                /* gis type */
        char name[MAXGISLAYER][256]; /* name */
        int flag[MAXGISLAYER];       /* flag */
        gisd_t *data[MAXGISLAYER];   /* gis data list */
        double bound[4];             /* boundary {lat0,lat1,lon0,lon1} */
    } gis_t;

    typedef void fatalfunc_t(const char *); /* fatal callback function type */

    /* global variables ----------------------------------------------------------*/
    extern const double chisqr[];    /* chi-sqr(n) table (alpha=0.001) */
    extern const double lam_carr[];  /* carrier wave length (m) {L1,L2,...} */
    extern const double ura_value[]; /* user range accuracy translation table */
    extern const double bdsmptable[35][5];
    extern const prcopt_t prcopt_default;     /* default positioning options */
    extern const solopt_t solopt_default;     /* default solution output options */
    extern const filopt_t filopt_default;     /* default solution output options */
    extern const sbsigpband_t igpband1[9][8]; /* SBAS IGP band 0-8 */
    extern const sbsigpband_t igpband2[2][5]; /* SBAS IGP band 9-10 */
    extern const char *formatstrs[];          /* stream format strings */
    extern opt_t sysopts[];                   /* system options table */

    /* satellites, systems, orb, codes functions --------------------------------------*/
    EXPORT int satno(int sys, int prn);
    EXPORT int satsys(int sat, int *prn);
    EXPORT void satorb(int sat, int *orb);
    EXPORT int satid2no(const char *id);
    EXPORT void satno2id(int sat, char *id);
    EXPORT uint8_t obs2code(const char *obs);
    EXPORT char *code2obs(uint8_t code);
    EXPORT double code2freq(int sys, uint8_t code, int fcn);
    EXPORT double sat2freq(int sat, uint8_t code, const nav_t *nav);
    EXPORT int code2idx(int sys, uint8_t code);
    EXPORT int satexclude(int sat, int svh, const prcopt_t *opt);
    EXPORT void setcodepri(int sys, int idx, const char *pri);
    EXPORT int getcodepri(int sys, uint8_t code, const char *opt);
    ;
    EXPORT int testsnr(int base, int idx, double el, double snr, const snrmask_t *mask);

    /* matrix and vector functions -----------------------------------------------*/
    EXPORT double *mat(int n, int m); /* front:lie  */
    EXPORT int *imat(int n, int m);
    EXPORT double *zeros(int n, int m);
    EXPORT double *eye(int n);
    EXPORT double dot(const double *a, const double *b, int n);
    EXPORT double norm(const double *a, int n);
    EXPORT void cross3(const double *a, const double *b, double *c);
    EXPORT int normv3(const double *a, double *b);
    EXPORT void intcpy(int *A, const int *B, int n, int m);
    EXPORT void matcpy(double *A, const double *B, int n, int m);
    EXPORT void chol(double *L, const double *P, int n);
    EXPORT void Linv(double *L, int n);
    EXPORT void matmul(const char *tr, int n, int k, int m, double alpha, const double *A, const double *B, double beta,
                       double *C);
    EXPORT int matinv(double *A, int n);
    EXPORT int solve(const char *tr, const double *A, const double *Y, int n, int m, double *X);
    EXPORT int lsq(const double *A, const double *y, int n, int m, double *x, double *Q);
    EXPORT int filter(double *x, double *P, const double *H, const double *v, const double *R, int n, int m);
    EXPORT int smoother(const double *xf, const double *Qf, const double *xb, const double *Qb, int n, double *xs,
                        double *Qs);
    EXPORT void matprint(const double *A, int n, int m, int p, int q);
    EXPORT void matfprint(const double *A, int n, int m, int p, int q, FILE *fp);

    EXPORT void add_fatal(fatalfunc_t *func);

    /* time and string functions -------------------------------------------------*/
    EXPORT double str2num(const char *s, int i, int n);
    EXPORT int str2time(const char *s, int i, int n, gtime_t *t);
    EXPORT void time2str(gtime_t t, char *str, int n);
    EXPORT gtime_t epoch2time(const double *ep);
    EXPORT void time2epoch(gtime_t t, double *ep);
    EXPORT gtime_t gpst2time(int week, double sec);
    EXPORT double time2gpst(gtime_t t, int *week);
    EXPORT gtime_t gst2time(int week, double sec);
    EXPORT double time2gst(gtime_t t, int *week);
    EXPORT gtime_t bdt2time(int week, double sec);
    EXPORT double time2bdt(gtime_t t, int *week);
    EXPORT char *time_str(gtime_t t, int n);

    EXPORT gtime_t timeadd(gtime_t t, double sec);
    EXPORT double timediff(gtime_t t1, gtime_t t2);
    EXPORT gtime_t gpst2utc(gtime_t t);
    EXPORT gtime_t utc2gpst(gtime_t t);
    EXPORT gtime_t gpst2bdt(gtime_t t);
    EXPORT gtime_t bdt2gpst(gtime_t t);
    EXPORT gtime_t timeget(void);
    EXPORT void timeset(gtime_t t);
    EXPORT double time2doy(gtime_t t);
    EXPORT double utc2gmst(gtime_t t, double ut1_utc);
    EXPORT int read_leaps(const char *file);

    EXPORT int adjgpsweek(int week);
    EXPORT unsigned int tickget(void);
    EXPORT void sleepms(int ms);

    EXPORT int reppath(const char *path, char *rpath, gtime_t time, const char *rov, const char *base);
    EXPORT int reppaths(const char *path, char *rpaths[], int nmax, gtime_t ts, gtime_t te, const char *rov,
                        const char *base);

    /* coordinates transformation ------------------------------------------------*/
    EXPORT void ecef2pos(const double *r, double *pos);
    EXPORT void pos2ecef(const double *pos, double *r);
    EXPORT void ecef2enu(const double *pos, const double *r, double *e);
    EXPORT void enu2ecef(const double *pos, const double *e, double *r);
    EXPORT void covenu(const double *pos, const double *P, double *Q);
    EXPORT void covecef(const double *pos, const double *Q, double *P);
    EXPORT void xyz2enu(const double *pos, double *E);
    EXPORT void eci2ecef(gtime_t tutc, const double *erpv, double *U, double *gmst);
    EXPORT void deg2dms(double deg, double *dms, int ndec);
    EXPORT double dms2deg(const double *dms);

    /* input and output functions ------------------------------------------------*/
    EXPORT void readpos(const char *file, const char *rcv, double *pos);
    EXPORT int sortobs(obs_t *obs);
    EXPORT void uniqnav(nav_t *nav);
    EXPORT int screent(gtime_t time, gtime_t ts, gtime_t te, double tint);
    EXPORT int readnav(const char *file, nav_t *nav);
    EXPORT int savenav(const char *file, const nav_t *nav);
    EXPORT void freeobs(obs_t *obs);
    EXPORT void freenav(nav_t *nav, int opt);
    EXPORT int readblq(const char *file, const char *sta, double *odisp);
    EXPORT int readerp(const char *file, erp_t *erp);
    EXPORT int geterp(const erp_t *erp, gtime_t time, double *val);
    EXPORT int readproduct(const prcopt_t *prcopt, const filopt_t *fopt, nav_t *nav, pcvs_t *pcvss, pcvs_t *pcvsr,
                           stas_t *stas);
    EXPORT void freeproduct(nav_t *nav, pcvs_t *pcvs, pcvs_t *pcvr, stas_t *stas);
    EXPORT int readobs(const char *infile, int rcv, const prcopt_t *prcopt, obs_t *obs, sta_t *sta, int *nepoch);
    EXPORT int readstas(const char *file, stas_t *stas);
    EXPORT void freestas(stas_t *stas);
    EXPORT void readsnx(const char *snxfile, const char *infile, const char *outfile);

    /* debug trace functions -----------------------------------------------------*/
    EXPORT void traceopen(const char *file);
    EXPORT void traceclose(void);
    EXPORT void tracelevel(int level);
    EXPORT void trace(int level, const char *format, ...);
    EXPORT void tracet(int level, const char *format, ...);
    EXPORT void tracemat(int level, const double *A, int n, int m, int p, int q);
    EXPORT void traceobs(int level, const obsd_t *obs, int n);
    EXPORT void tracenav(int level, const nav_t *nav);
    EXPORT void tracegnav(int level, const nav_t *nav);
    EXPORT void tracehnav(int level, const nav_t *nav);
    EXPORT void tracepeph(int level, const nav_t *nav);
    EXPORT void tracepclk(int level, const nav_t *nav);
    EXPORT void traceb(int level, const unsigned char *p, int n);

    /* platform dependent functions ----------------------------------------------*/
    EXPORT int execcmd(const char *cmd);
    EXPORT int expath(const char *path, char *paths[], int nmax);
    EXPORT void createdir(const char *path);

    /* positioning models --------------------------------------------------------*/
    EXPORT double satazel(const double *pos, const double *e, double *azel);
    EXPORT double geodist(const double *rs, const double *rr, double *e);
    EXPORT void dops(int ns, const double *azel, double elmin, double *dop);
    EXPORT void csmooth(obs_t *obs, int ns);

    /* atmosphere models ---------------------------------------------------------*/
    EXPORT double ionmodel(gtime_t t, const double *ion, const double *pos, const double *azel);
    EXPORT double ionmapf(const double *pos, const double *azel);
    EXPORT double ionppp(const double *pos, const double *azel, double re, double hion, double *pppos);
    EXPORT double tropmodel(gtime_t time, const double *pos, const double *azel, int metopt, double humi, int outopt);
    EXPORT double tropmapf(gtime_t time, const double *pos, const double *azel, int mfopt, double *mapfw);
    EXPORT int iontec(gtime_t time, const nav_t *nav, const double *pos, const double *azel, int opt, double *delay,
                      double *var);
    EXPORT void readtec(const char *file, nav_t *nav, int opt);

    /* antenna models ------------------------------------------------------------*/
    EXPORT int readpcv(const char *file, pcvs_t *pcvs);
    EXPORT pcv_t *searchpcv(int sat, const char *type, gtime_t time, const pcvs_t *pcvs);
    EXPORT void antmodel(const pcv_t *pcv, const double *del, const double *azel, int opt, double *dant);
    EXPORT void antmodel_s(const pcv_t *pcv, double nadir, double *dant);
    EXPORT int model_phw(gtime_t time, int sat, const char *type, int opt, const double *rs, const double *rr,
                         double *phw);

    /* earth tide models ---------------------------------------------------------*/
    EXPORT void sunmoonpos(gtime_t tutc, const double *erpv, double *rsun, double *rmoon, double *gmst);
    EXPORT void tidedisp(gtime_t tutc, const double *rr, int opt, const erp_t *erp, const double *odisp, double *dr);

    /* geiod models --------------------------------------------------------------*/
    EXPORT int opengeoid(int model, const char *file);
    EXPORT void closegeoid(void);
    EXPORT double geoidh(const double *pos);

    /* datum transformation ------------------------------------------------------*/
    EXPORT int loaddatump(const char *file);
    EXPORT int tokyo2jgd(double *pos);
    EXPORT int jgd2tokyo(double *pos);

    /* rinex functions -----------------------------------------------------------*/
    EXPORT int readrnx(const char *file, int rcv, const char *opt, obs_t *obs, nav_t *nav, sta_t *sta);
    EXPORT int readrnxt(const char *file, int rcv, gtime_t ts, gtime_t te, double tint, const char *opt, obs_t *obs,
                        nav_t *nav, sta_t *sta);
    EXPORT int readrnxc(const char *file, nav_t *nav);
    EXPORT int outrnxobsh(FILE *fp, const rnxopt_t *opt, const nav_t *nav);
    EXPORT int outrnxobsb(FILE *fp, const rnxopt_t *opt, const obsd_t *obs, int n, int epflag);
    EXPORT int outrnxnavh(FILE *fp, const rnxopt_t *opt, const nav_t *nav);
    EXPORT int outrnxgnavh(FILE *fp, const rnxopt_t *opt, const nav_t *nav);
    EXPORT int outrnxhnavh(FILE *fp, const rnxopt_t *opt, const nav_t *nav);
    EXPORT int outrnxlnavh(FILE *fp, const rnxopt_t *opt, const nav_t *nav);
    EXPORT int outrnxqnavh(FILE *fp, const rnxopt_t *opt, const nav_t *nav);
    EXPORT int outrnxcnavh(FILE *fp, const rnxopt_t *opt, const nav_t *nav);
    EXPORT int outrnxinavh(FILE *fp, const rnxopt_t *opt, const nav_t *nav);
    EXPORT int outrnxnavb(FILE *fp, const rnxopt_t *opt, const eph_t *eph);
    EXPORT int outrnxgnavb(FILE *fp, const rnxopt_t *opt, const geph_t *geph);
    EXPORT int outrnxhnavb(FILE *fp, const rnxopt_t *opt, const seph_t *seph);
    EXPORT int rtk_uncompress(const char *file, char *uncfile);
    EXPORT int convrnx(int format, rnxopt_t *opt, const char *file, char **ofile);
    EXPORT int init_rnxctr(rnxctr_t *rnx);
    EXPORT void free_rnxctr(rnxctr_t *rnx);
    EXPORT int open_rnxctr(rnxctr_t *rnx, FILE *fp);
    EXPORT int input_rnxctr(rnxctr_t *rnx, FILE *fp);

    /* ephemeris and clock functions ---------------------------------------------*/
    EXPORT double eph2clk(gtime_t time, const eph_t *eph);
    EXPORT double geph2clk(gtime_t time, const geph_t *geph);
    EXPORT double seph2clk(gtime_t time, const seph_t *seph);
    EXPORT double uravalue(int ura, int sys);
    EXPORT void eph2pos(gtime_t time, const eph_t *eph, double *rs, double *dts, double *var);
    EXPORT void geph2pos(gtime_t time, const geph_t *geph, double *rs, double *dts, double *var);
    EXPORT void seph2pos(gtime_t time, const seph_t *seph, double *rs, double *dts, double *var);
    EXPORT int peph2pos(gtime_t time, int sat, const nav_t *nav, int opt, double *rs, double *dts, double *var);
    EXPORT void satantoff(gtime_t time, const double *rs, int sat, const nav_t *nav, double *dant);
    EXPORT int satpos(gtime_t time, gtime_t teph, int sat, int ephopt, const nav_t *nav, double *rs, double *dts,
                      double *var, int *svh);
    EXPORT void satposs(gtime_t time, const obsd_t *obs, int n, const nav_t *nav, int sateph, double *rs, double *dts,
                        double *var, int *svh);
    EXPORT void setseleph(int sys, int sel);
    EXPORT int getseleph(int sys);
    EXPORT void readsp3(const char *file, nav_t *nav, int opt);
    EXPORT int readsap(const char *file, gtime_t time, nav_t *nav);
    EXPORT int readdcb(const char *file, nav_t *nav);
    EXPORT void alm2pos(gtime_t time, const alm_t *alm, double *rs, double *dts);

    /* receiver raw data functions -----------------------------------------------*/
    EXPORT uint32_t getbitu(const uint8_t *buff, int pos, int len);
    EXPORT int32_t getbits(const uint8_t *buff, int pos, int len);
    EXPORT void setbitu(uint8_t *buff, int pos, int len, uint32_t data);
    EXPORT void setbits(uint8_t *buff, int pos, int len, int32_t data);
    EXPORT unsigned int rtk_crc32(const unsigned char *buff, int len);
    EXPORT unsigned int rtk_crc24q(const unsigned char *buff, int len);
    EXPORT unsigned short rtk_crc16(const unsigned char *buff, int len);
    EXPORT int decode_word(unsigned int word, unsigned char *data);

    /* solution functions --------------------------------------------------------*/
    EXPORT void initsolbuf(solbuf_t *solbuf, int cyclic, int nmax);
    EXPORT void freesolbuf(solbuf_t *solbuf);
    EXPORT void freesolstatbuf(solstatbuf_t *solstatbuf);
    EXPORT sol_t *getsol(solbuf_t *solbuf, int index);
    EXPORT int addsol(solbuf_t *solbuf, const sol_t *sol);
    EXPORT int readsol(char *files[], int nfile, solbuf_t *sol);
    EXPORT int readsolt(char *files[], int nfile, gtime_t ts, gtime_t te, double tint, int qflag, solbuf_t *sol);
    EXPORT int readsolstat(char *files[], int nfile, solstatbuf_t *statbuf);
    EXPORT int readsolstatt(char *files[], int nfile, gtime_t ts, gtime_t te, double tint, solstatbuf_t *statbuf);
    EXPORT int inputsol(unsigned char data, gtime_t ts, gtime_t te, double tint, int qflag, const solopt_t *opt,
                        solbuf_t *solbuf);

    EXPORT int outprcopts(unsigned char *buff, const prcopt_t *opt);
    EXPORT int outsolheads(unsigned char *buff, const solopt_t *opt);
    EXPORT int outsols(unsigned char *buff, const sol_t *sol, const double *rb, const solopt_t *opt);
    EXPORT int outsolexs(unsigned char *buff, const sol_t *sol, const ssat_t *ssat, const solopt_t *opt);
    EXPORT void outprcopt(FILE *fp, const prcopt_t *opt);
    EXPORT void outsolhead(FILE *fp, const solopt_t *opt);
    EXPORT void outsol(FILE *fp, const sol_t *sol, const double *rb, const solopt_t *opt);
    EXPORT void outsolex(FILE *fp, const sol_t *sol, const ssat_t *ssat, const solopt_t *opt);
    EXPORT void outheader(FILE *fp, const prcopt_t *popt, const solopt_t *sopt);
    EXPORT int outnmea_rmc(unsigned char *buff, const sol_t *sol);
    EXPORT int outnmea_gga(unsigned char *buff, const sol_t *sol);
    EXPORT int outnmea_gsa(unsigned char *buff, const sol_t *sol, const ssat_t *ssat);
    EXPORT int outnmea_gsv(unsigned char *buff, const sol_t *sol, const ssat_t *ssat);
    EXPORT int convkml(const char *infile, const char *outfile, gtime_t ts, gtime_t te, double tint, int qflg,
                       int tcolor, int pcolor, int outalt, int outtime);
    EXPORT int sol2kml(char *path);

    /* options functions ---------------------------------------------------------*/
    EXPORT opt_t *searchopt(const char *name, const opt_t *opts);
    EXPORT int str2opt(opt_t *opt, const char *str);
    EXPORT int opt2str(const opt_t *opt, char *str);
    EXPORT int opt2buf(const opt_t *opt, char *buff);
    EXPORT int loadopts(const char *file, opt_t *opts);
    EXPORT int saveopts(const char *file, const char *mode, const char *comment, const opt_t *opts);
    EXPORT void resetsysopts(void);
    EXPORT void getsysopts(prcopt_t *popt, solopt_t *sopt, filopt_t *fopt);
    EXPORT void setsysopts(const prcopt_t *popt, const solopt_t *sopt, const filopt_t *fopt);

    /* stream data input and output functions ------------------------------------*/
    EXPORT void strinitcom(void);
    EXPORT void strinit(stream_t *stream);
    EXPORT void strlock(stream_t *stream);
    EXPORT void strunlock(stream_t *stream);
    EXPORT int stropen(stream_t *stream, int type, int mode, const char *path);
    EXPORT void strclose(stream_t *stream);
    EXPORT int strread(stream_t *stream, unsigned char *buff, int n);
    EXPORT int strwrite(stream_t *stream, unsigned char *buff, int n);
    EXPORT void strsync(stream_t *stream1, stream_t *stream2);
    EXPORT int strstat(stream_t *stream, char *msg);
    EXPORT int strstatx(stream_t *stream, char *msg);
    EXPORT void strsum(stream_t *stream, int *inb, int *inr, int *outb, int *outr);
    EXPORT int strgetsel(stream_t *stream, char *sel);
    EXPORT int strsetsel(stream_t *stream, const char *sel);
    EXPORT int strsetsrctbl(stream_t *stream, const char *file);
    EXPORT void strsetopt(const int *opt);
    EXPORT gtime_t strgettime(stream_t *stream);
    EXPORT void strsendnmea(stream_t *stream, const sol_t *sol);
    EXPORT void strsendcmd(stream_t *stream, const char *cmd);
    EXPORT void strsettimeout(stream_t *stream, int toinact, int tirecon);
    EXPORT void strsetdir(const char *dir);
    EXPORT void strsetproxy(const char *addr);

    /* integer ambiguity resolution ----------------------------------------------*/
    EXPORT int lambda(int n, int m, const double *a, const double *Q, double *F, double *s);
    EXPORT int lambda_reduction(int n, const double *Q, double *Z);
    EXPORT int lambda_search(int n, int m, const double *a, const double *Q, double *F, double *s);
    EXPORT int lambda_PAR(rtk_t *rtk, int na, int nb, int m, const double *a, double *b, const double *Qaa, double *Qbb,
                          const double *Qba);
    EXPORT double normcdf(double x);

    /* standard positioning ------------------------------------------------------*/
    EXPORT int pntpos(const obsd_t *obs, int n, const nav_t *nav, const prcopt_t *opt, sol_t *sol, double *azel,
                      ssat_t *ssat, char *msg);

    /* precise positioning -------------------------------------------------------*/
    EXPORT void rtkinit(rtk_t *rtk, const prcopt_t *opt);
    EXPORT void rtkfree(rtk_t *rtk);
    EXPORT void relpos(rtk_t *rtk, const obsd_t *obs, int nu, int nr, const nav_t *nav);
    EXPORT void ppppos(rtk_t *rtk, const obsd_t *obs, int n, const nav_t *nav, const double *xg, const double *Pg);
    EXPORT void outppp(FILE *fp, const rtk_t *rtk, const obsd_t *obs, int n, const nav_t *nav);
    EXPORT void outrel(FILE *fp, const rtk_t *rtk);
    EXPORT int relnx(const prcopt_t *opt);
    EXPORT int pppnx(const prcopt_t *opt);

    /* precise point positioning -------------------------------------------------*/
    EXPORT int ppparopenstat(const char *file);
    EXPORT int pppamb(rtk_t *rtk, const obsd_t *obs, int n, const nav_t *nav);

    /* extern local constraint -------------------------------------------------*/
    EXPORT void decode_corr(const char *file, nav_t *nav, int dt, int opt);
    EXPORT int pppcorr_read(const char *files, nav_t *nav);
    EXPORT int pppcorr_trop(gtime_t time, const double *blh, double *trp, double *var, const nav_t *nav);
    EXPORT int pppcorr_stat(gtime_t time, const double *blh, const int sat, const nav_t *nav);
    EXPORT int pppcorr_stec(gtime_t time, const double *blh, const int sat1, const int sat2, double *stec, double *var,
                            const nav_t *nav, int opt);

    /* stream server functions ---------------------------------------------------*/
    EXPORT void strsvrinit(strsvr_t *svr, int nout);
    EXPORT int strsvrstart(strsvr_t *svr, int *opts, int *strs, char **paths, strconv_t **conv, char **cmds,
                           char **cmds_priodic, const double *nmeapos);
    EXPORT void strsvrstop(strsvr_t *svr, char **cmds);
    EXPORT void strsvrstopold(strsvr_t *svr, char *cmds);
    EXPORT void strsvrstat(strsvr_t *svr, int *stat, int *byte, int *bps, char *msg);
    EXPORT strconv_t *strconvnew(int itype, int otype, const char *msgs, int staid, int stasel, const char *opt);
    EXPORT void strconvfree(strconv_t *conv);
    EXPORT void strsvrsetsrctbl(strsvr_t *svr, const char *file);

    /* downloader functions ------------------------------------------------------*/
    EXPORT int dl_readurls(const char *file, char **types, int ntype, url_t *urls, int nmax);
    EXPORT int dl_readstas(const char *file, char **stas, int nmax);
    EXPORT int dl_exec(gtime_t ts, gtime_t te, double ti, int seqnos, int seqnoe, const url_t *urls, int nurl,
                       char **stas, int nsta, const char *dir, const char *usr, const char *pwd, const char *proxy,
                       int opts, char *msg, FILE *fp);
    EXPORT void dl_test(gtime_t ts, gtime_t te, double ti, const url_t *urls, int nurl, char **stas, int nsta,
                        const char *dir, int ncol, int datefmt, FILE *fp);
    EXPORT int loadobs(gtime_t ts, gtime_t te, const char *infile, const char *outdir);

    /* functions ----------------------------------------------------------------*/
    EXPORT double median(const double *data, int n);
    EXPORT double bd2smp(int orb, double *azel, int nq);
    EXPORT void setpcv(gtime_t time, prcopt_t *popt, nav_t *nav, const pcvs_t *pcvs, const pcvs_t *pcvr,
                       const sta_t *sta);
    EXPORT int ionocorr(gtime_t time, const double *pos, const double *azel, const prcopt_t *opt, int sat,
                        const double *x, const nav_t *nav, double *dion, double *var);
    EXPORT int tropcorr(gtime_t time, const double *pos, const double *azel, const prcopt_t *opt, const double *x,
                        double *dtdx, const nav_t *nav, double *dtrp, double *var);
    EXPORT void corr_meas(const obsd_t *obs, const nav_t *nav, const double *azel, const prcopt_t *opt,
                          const double *dantr, const double *dants, double phw, const double php, double *L, double *P,
                          double *Lc, double *Pc);
    EXPORT double varerr(unsigned char sat, double el, int freq, int type, const prcopt_t *opt);
    EXPORT double varerrSD(int sat, int sys, double el, double bl, double dt, int f, const prcopt_t *opt);
    EXPORT int readfcb_sgg(const char *file, nav_t *nav);
    EXPORT int readifcb_sgg(const char *file, nav_t *nav);
    EXPORT int drawnlfcb(const char *infile, const char *outfile);

    /* trop function ------------------------------------------------------------ */
    EXPORT void nmf(gtime_t time, const double pos[], const double azel[], double *mapfh, double *mapfw);
    EXPORT void gmf(double *mjd, double *lat, double *lon, double *hgt, double *zd, double *gmfh, double *gmfw);

    EXPORT int gpt(double *mjd, double *lat, double *lon, double *hgt, double *pres, double *temp, double *undu);

    /* preprocessing ------------------------------------------------------------ */
    EXPORT int tdcpos(const obsd_t *obs, int n, const nav_t *nav, const prcopt_t *opt, const double *rs,
                      const double *dts, const int *svh, sol_t *sol, ssat_t *ssat);

    /* application defined functions -------------------------------------------- */
    extern int showmsg(char *format, ...);
    extern void settspan(gtime_t ts, gtime_t te);
    extern void settime(gtime_t time);
    extern int checkbrk(const char *format, ...);

#ifdef __cplusplus
}
#endif
#endif /* RTKLIB_H */
