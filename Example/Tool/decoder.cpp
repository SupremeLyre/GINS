#include <array>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#define PI 3.1415926535897932 /* pi */
#define D2R (PI / 180.0)      /* deg to rad */
#define R2D (180.0 / PI)      /* rad to deg */
typedef struct
{
    std::array<double, 3> att;
    std::array<double, 3> vn;
    std::array<double, 3> pos;
    std::array<double, 9> Xk;
    std::array<double, 3> eb;
    std::array<double, 3> db;
    std::array<double, 15> Pk;
    double kftk;
} InsResult;
std::string binfilename = "kfdebug-urban-f2.bin";
int main(int argc, char **argv)
{
    std::string path = "./";
    if (argc > 1)
        path = argv[1];
    path += binfilename;
    std::cout << "decode psins result:\n" << path << "\n";
    std::fstream fin(path, std::ios::in | std::ios::binary);
    std::fstream fout(path + ".txt", std::ios::out);
    InsResult temp{};
    while (!fin.eof())
    {
        char line[8192] = "";
        for (int idx = 0; idx < (int)temp.att.size(); idx++)
        {
            fin.read((char *)&temp.att[idx], sizeof(double));
            sprintf(line, "%13.8lf, ", temp.att[idx] * R2D);
            fout << line;
        }
        for (int idx = 0; idx < (int)temp.vn.size(); idx++)
        {
            fin.read((char *)&temp.vn[idx], sizeof(double));
            sprintf(line, "%10.5lf, ", temp.vn[idx]);
            fout << line;
        }
        for (int idx = 0; idx < (int)temp.pos.size(); idx++)
        {
            fin.read((char *)&temp.pos[idx], sizeof(double));
            sprintf(line, "%14.9lf, ", (idx == 2 ? temp.pos[idx] : temp.pos[idx] * R2D));
            fout << line;
        }
        for (int idx = 0; idx < (int)temp.Xk.size(); idx++)
        {
            fin.read((char *)&temp.Xk[idx], sizeof(double));
            sprintf(line, "%10.5lf, ", temp.Xk[idx]);
            fout << line;
        }
        for (int idx = 0; idx < (int)temp.eb.size(); idx++)
        {
            fin.read((char *)&temp.eb[idx], sizeof(double));
            sprintf(line, "%10.5lf, ", temp.eb[idx]);
            fout << line;
        }

        for (int idx = 0; idx < (int)temp.db.size(); idx++)
        {
            fin.read((char *)&temp.db[idx], sizeof(double));
            sprintf(line, "%10.5lf, ", temp.db[idx]);
            fout << line;
        }
        for (int idx = 0; idx < (int)temp.Pk.size(); idx++)
        {
            fin.read((char *)&temp.Pk[idx], sizeof(double));
            sprintf(line, "%10.5lf, ", temp.Pk[idx]);
            fout << line;
        }
        fin.read((char *)&temp.kftk, sizeof(double));
        sprintf(line, "%10.5lf\n", temp.kftk);
        fout << line;
        // std::cout << temp.att[0] << " " << temp.att[1] << " " << temp.att[2] << std::endl;
    }
    fin.clear();
    fout.clear();
    fin.close();
    fout.close();
    return 0;
}