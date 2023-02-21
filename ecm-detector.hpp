// File        ecm-detector.hpp
// Summary     Detection algorithm using my forward model of the event camera
// Author      Jacob Trzaska
# pragma once

// Standard imports
# include <iostream>
# include <vector>
# include <string>
# include <cmath>

// Other imports
# include "base-classes.hpp"


class ECM : public Detector {
     /*
     This class implements object detection using the forward model
     that I developed for the DAVIS event cameras.
     */
     private:
     double tau;


     public:
     ECM() {



     }

     ~ECM() {}

     
     Eigen::MatrixXd processSensorData(double * events, int N) {
          /*
          Run detection on an event stream.

          Args:
               events: Pointer to the event stream. The stream should have the following
                       data layout - [t1x1y1p1 t2x2y2p2 ... tNxNyNpN] where ti, xi, yi, 
                       and pi are the timestamp, x-coordinate, y-coordinate, and polarity
                       of the the ith event.
               N: The number of events in the stream.

          Ret:
               A matrix whose rows give the spatial location of possibly interesting objects.
          */
          processTimestamps(events, N);

          return Eigen::MatrixXd::Zero(1,1); // temp return value
     }

     private:
     void processTimestamps(double * events, int N) {
          /*
          Convert the timestamps into units of a pre-determined characteristic time.

          Args:
               events: Event stream.
               N: The number of events reported.

          Ret:
               The event data with rescaled timestamps.
          */
          for (int i {0}; i < N; ++i)
               events[4 * i] = events[4 * i] / tau;

          return;
    }
};
