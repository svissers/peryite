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
 *  Copyright 2018, Kuylen E, Willem L, Broeckhove J
 */

/**
 * @file
 * Definition of Observer for SimEvents translated to a visual map
 */

#include "MapViewer.h"

#include "calendar/Calendar.h"
#include "pop/Population.h"
#include "sim/SimRunner.h"
#include "sim/Sim.h"

using namespace std;
using namespace stride::sim_event;

namespace stride {
namespace viewers {

void MapViewer::Update(sim_event::Id id)
{
        switch (id) {
        case Id::AtStart: {
                // -----------------------------------------------------------------------------------------
                // Create the Qt app
                // -----------------------------------------------------------------------------------------
                // The QApplication constructor needs argc and argv, but we don't use those,
                // so we make some here (because we're not in main()).
                // -----------------------------------------------------------------------------------------
                // Also, normally QApplication.exec() is used, but since that is blocking, we will use
                // processEvents() manually, so we can use it on every notification.
                // When we're done, we call exec() because the simulator no longer needs to do anything.

                int argc = 1;
                char *argv[1] = { (char *)"" };
                m_qapp = new QApplication(argc, argv);
                mvw = new MapViewerWindow;
                mvw->show();

                m_qapp->processEvents();

                const auto pop = m_runner->GetSim()->GetPopulation();
                mvw->createPopView(pop);
                mvw->updateDaysLabel(0);
                mvw->focusFlanders();

                m_qapp->processEvents();

                break;
        }
        case Id::Stepped: {
                const auto pop = m_runner->GetSim()->GetPopulation();
                mvw->updateInfected(pop);

                int days = m_runner->GetSim()->GetCalendar()->GetSimulationDay();
                mvw->updateDaysLabel(days);

                m_qapp->processEvents();

                break;
        }
        case Id::Finished: {
                const auto pop = m_runner->GetSim()->GetPopulation();
                mvw->updateInfected(pop);
                m_qapp->exec();

                break;
        }
        default: break;
        }
}

} // namespace viewers
} // namespace stride

