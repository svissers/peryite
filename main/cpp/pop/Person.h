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
 * Header file for the Person class.
 */

#include "disease/Health.h"
#include "pool/ContactPoolType.h"
#include "pool/IdSubscriptArray.h"
#include "util/GeometryGeoCoord.h"

#include <boost/property_tree/ptree.hpp>

namespace stride {

class Belief;

/**
 * Store and handle person data.
 */
class Person
{
public:
        /// Default construction (for population vector).
        Person()
            : m_age(0.0), m_belief(nullptr), m_gender('M'), m_health(), m_id(0), m_is_participant(), m_pool_ids(), m_in_pools(), m_coord(util::spherical_point(0,0))
        {
        }

        /// Constructor: set the person data.
        Person(unsigned int id, double age, unsigned int householdId, unsigned int schoolId, unsigned int workId,
               unsigned int primaryCommunityId, unsigned int secondaryCommunityId, double latitude, double longitude)
            : m_age(age), m_belief(nullptr), m_gender('M'), m_health(), m_id(id), m_is_participant(false),
              m_pool_ids{householdId, schoolId, workId, primaryCommunityId, secondaryCommunityId}, m_in_pools(true), m_coord(util::spherical_point(latitude, longitude))
        {
        }

        /// Is this person not equal to the given person?
        bool operator!=(const Person& p) const { return p.m_id != m_id; }

        /// Get the age.
        double GetAge() const { return m_age; }

        /// Return belief info.
        Belief* GetBelief() { return m_belief; }

        /// Return belief info.
        const Belief* GetBelief() const { return m_belief; }

        /// Get ID of contactpool_type
        unsigned int GetPoolId(const ContactPoolType::Id& poolType) const { return m_pool_ids[poolType]; }

        /// Return person's gender.
        char GetGender() const { return m_gender; }

        /// Return person's health status.
        Health& GetHealth() { return m_health; }

        /// Return person's health status.
        const Health& GetHealth() const { return m_health; }

        /// Get the id.
        unsigned int GetId() const { return m_id; }

        // Get the household coordinate
        util::spherical_point GetCoordinate() const { return m_coord; }

        // Set the household coordinate
        void SetCoordinate(util::spherical_point coord) { m_coord = coord; }

        /// Check if a person is present today in a given contactpool
        bool IsInPool(const ContactPoolType::Id& poolType) const { return m_in_pools[poolType]; }

        /// Does this person participates in the social contact study?
        bool IsSurveyParticipant() const { return m_is_participant; }

        /// Participate in social contact study and log person details
        void ParticipateInSurvey() { m_is_participant = true; }

        /// Set the beliefs. Pointer into Population's beliefcontainer.
        void SetBelief(Belief* belief) { m_belief = belief; };

        /// Update the health status and presence in contactpools.
        void Update(bool isWorkOff, bool isSchoolOff);

        ///
        void Update(Person* p);

        /// Set ID of contactpool_type
        void setPoolId(const ContactPoolType::Id& pool_type, unsigned int id) { m_pool_ids[pool_type] = id; }

private:
        double       m_age;            ///< The age.
        Belief*      m_belief;         ///< Health beliefs related data (raw pointer intentional).
        char         m_gender;
        Health       m_health;         ///< Health info for this person.
        unsigned int m_id;             ///< The id.
        bool         m_is_participant; ///< Is participating in the social contact study

        ///< Ids (school, work, etc) of pools you belong to Id value 0 means you do not belong to any
        ///< pool of that type (e.g. school and work are mutually exclusive.
        ContactPoolType::IdSubscriptArray<unsigned int> m_pool_ids;

        ///< Is person present/absent in pools of each of the types (school, work, etc)?
        ContactPoolType::IdSubscriptArray<bool> m_in_pools;
        util::spherical_point m_coord; ///< The household coordinates
};

} // namespace stride
