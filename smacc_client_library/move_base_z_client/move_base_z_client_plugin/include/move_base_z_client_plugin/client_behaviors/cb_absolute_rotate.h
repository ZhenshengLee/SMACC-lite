/*****************************************************************************************************************
 * ReelRobotix Inc. - Software License Agreement      Copyright (c) 2018
 * 	 Authors: Pablo Inigo Blasco, Brett Aldrich
 *
 ******************************************************************************************************************/
#pragma once

#include "cb_move_base_client_behavior_base.h"
#include <boost/optional.hpp>
#include <tf/transform_listener.h>

namespace cl_move_base_z
{
    class CbAbsoluteRotate : public CbMoveBaseClientBehaviorBase
    {
    public:
        enum class SpiningPlanner
        {
            Default,
            PureSpinning,
            Forward
        };

        tf::TransformListener listener;

        boost::optional<float> absoluteGoalAngleDegree;
        boost::optional<float> yawGoalTolerance;
        boost::optional<SpiningPlanner> spinningPlanner;

        CbAbsoluteRotate();

        CbAbsoluteRotate(float absoluteGoalAngleDegree, float yawGoalTolerance = 0.15);

        virtual void onEntry() override;
        virtual void onExit() override;

    private:
        void setLocalPlannerYawTolerance(float newtolerance);
        float oldYawTolerance;
    };
} // namespace cl_move_base_z
