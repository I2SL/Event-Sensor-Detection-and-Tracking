// File        giftest.cpp
// Summary     Make a GIF of the event stream.
// Author      Jacob Trzaska

// Standard imports
# include <iostream>
# include <string>

// Other imports
# include "gifMaker.hpp"
# include "../base-classes.hpp"
# include "../Algorithm.hpp"

int main(int argc, char ** argv) {
     /*
     Make a GIF from event data.

     Args:
          argv[1]: File containing the event data.
          argv[2]: File containing the ground truth data.
          argv[3]: Integration time.

     Ret:
          0
     */
     std::string eData {argv[1]};
     std::string wAlgo {argv[2]};
     double time {std::stod(argv[3])};

     GifMaker gm(eData, time);

     /**Create an Algorithm object here.**/ 
     // DBSCAN-KNN & DBSCAN-GMPHD 
     Eigen::MatrixXd invals {Eigen::MatrixXd::Zero(1, 3)};
     invals(0, 0) = 8;
     invals(0, 1) = 8;
     invals(0, 2) = 1.2;

     // MS-KNN
     //Eigen::MatrixXd invals {Eigen::MatrixXd::Zero(1, 4)};
     //invals(0,0) = 5.2;
     //invals(0,1) = 9;
     //invals(0,2) = 74;
     //invals(0,3) = 1.2;

     // Model initializer
     double DT = time;
     double p3 = pow(DT, 3) / 3;
     double p2 = pow(DT, 2) / 2;

     Eigen::MatrixXd P {{16, 0, 0, 0}, {0, 16, 0, 0}, {0, 0, 9, 0}, {0, 0, 0, 9}};
     Eigen::MatrixXd F {{1, 0, DT, 0}, {0, 1, 0, DT}, {0, 0, 1, 0}, {0, 0, 0, 1}};
     Eigen::MatrixXd Q {{p3, 0, p2, 0}, {0, p3, 0, p2}, {p2, 0, DT, 0}, {0, p2, 0, DT}};
     Eigen::MatrixXd H {{1, 0, 0, 0}, {0, 1, 0, 0}};
     Eigen::MatrixXd R {{7, 0}, {0, 7}};

     // Define model for DBSCAN- and MS-KNN.
     KModel   k_model {.dt = DT, .P = P, .F = F, .Q = Q, .H = H, .R = R};
     // Define model for DBSCAN-GMPHD.
     std::vector<Eigen::MatrixXd> Fs, ds, Qs;
     std::vector<double> ws;
     Eigen::MatrixXd mean {{211}, {62}, {0}, {0}};
     Eigen::MatrixXd gcov {{12, 0, 0, 0}, {0, 12, 0, 0}, {0, 0, 5, 0}, {0, 0, 0, 5}};
     Gaussian gauss0(1.0, mean, gcov);
     std::vector<Gaussian> mix {gauss0};
     phdModel model {.dt = DT, .F = F, .Q = Q, .H = H, .R = R, .T = 1e-4, .U = 12, .Jmax = 45, .F_spawn = Fs, .w_spawn = ws,
                         .d_spawn = ds, .Q_spawn = Qs, .pS = 0.98, .pD = 0.75, .clutterIntensity = 2e-7, .initMix = mix};

     // Algo initializer
//     DBSCAN_KNN algo(invals, k_model);     
//     MS_KNN algo(invals, k_model);
       DBSCAN_GMPHD algo(invals, model);
     /***********************************/

     if (wAlgo != "y")
          gm.run("testfile.txt");
     else
          gm.run(algo, "eframes.txt", "tframes.txt");

     return 0;
}
