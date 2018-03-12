
/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2017, Kuylen E, Willem L, Broeckhove J
 */

/**
 * @file
 * Alias method implementation.
 */

#include "DiscreteSampler.h"

using namespace stride;
using namespace util;

DiscreteSampler::DiscreteSampler(const vector<double>& probabilities) {
    const double factor = 1.0 / std::accumulate(probabilities.begin(), probabilities.end(), 0.0);

    const auto n = probabilities.size();
    probs(n + 1);
    double tracker = 0;
    probs[0] = 0;
    for (unsigned int i = 1; i < n; i++){
        probs[i] = tracker + probabilities[i - 1] * factor;
        tracker = probs[i];
    }

}

unsigned int DiscreteSampler::Draw(double drawnNr) {
    for(unsigned int ProbInterval = 0; ProbInterval < probs.size();ProbInterval++){
        if(drawnNr < probs[ProbInterval+1])
            return ProbInterval;
    }

}
