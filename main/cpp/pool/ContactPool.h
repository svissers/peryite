#pragma once
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
 * Header for the core ContactPool class.
 */

#include "contact/ContactLogMode.h"
#include "pool/ContactPoolType.h"
#include "util/GeometryGeoCoord.h"

#include <tuple>
#include <vector>


namespace stride {

class Person;

/**
 * Represents a group of Persons that potentially have contacts.
 */
class ContactPool
{
public:
        /// Initializing constructor.
        ContactPool(std::size_t pool_id, ContactPoolType::Id type, util::spherical_point coord);

        /// Add the given Person.
        void AddMember(const Person* p);

        /// Get member at index.
        Person* GetMember(unsigned int index) const { return m_members[index]; }

        /// Get the entire pool of members.
        const std::vector<Person*>& GetPool() const { return m_members; }

        /// Get size (number of members).
        std::size_t GetSize() const { return m_members.size(); }

        // Get identifier
        std::size_t GetId() const { return m_pool_id;}

        // Get the coordinate of this contactpool
        util::spherical_point GetCoordinate() const { return m_coord; }

        // Set the coordinate of this contactpool
        void SetCoordinate(util::spherical_point coord) { m_coord = coord; }

        void UpdateTravel(const Person* person); /// Checks if an end travel date is reached and removes every person from that date out of the contactpool, it also calls person->ReturnHome()

        void AddTraveller(const Person* person); /// adds a traveller from a different region to the contactpool

private:
        /// Sort w.r.t. health status: order: exposed/infected/recovered, susceptible, immune.
        std::tuple<bool, size_t> SortMembers();

        /// Infector calculates contacts and transmissions.
        template <ContactLogMode::Id LL, bool TIC, typename LIP, bool TO>
        friend class Infector;

private:
        std::size_t          m_pool_id;      ///< The ID of the ContactPool (for logging purposes).
        ContactPoolType::Id  m_pool_type;    ///< The type of the ContactPool (for logging purposes).
        std::size_t          m_index_immune; ///< Index of the first immune member in the ContactPool.
        std::vector<Person*> m_members;      ///< Pointers to contactpool members (raw pointers intentional).
        util::spherical_point m_coord; ///< The household coordinates
        std::map<size_t, std::vector<unsigned int> > m_end_travel_dates; /// A map of end travel dates containing the person ids for that date (Simdays)
};

} // namespace stride
