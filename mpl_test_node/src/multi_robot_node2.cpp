#include "bag_writter.hpp"
#include "robot.hpp"
#include <decomp_ros_utils/data_ros_utils.h>
#include <mpl_external_planner/poly_map_planner/poly_map_planner.h>
#include <planning_ros_utils/data_ros_utils.h>
#include <planning_ros_utils/primitive_ros_utils.h>
#include <ros/ros.h>

using namespace MPL;

vec_E<Polyhedron2D> set_obs(vec_E<Robot<2>>& robots, decimal_t time,
                            const vec_E<PolyhedronObstacle2D>& external_static_obs) {
  vec_E<Polyhedron2D> poly_obs;
  for(size_t i = 0; i < robots.size(); i++) {
    vec_E<PolyhedronLinearObstacle2D> linear_obs;
    vec_E<PolyhedronNonlinearObstacle2D> nonlinear_obs;
    for(size_t j = 0; j < robots.size(); j++) {
      if(i != j)
        nonlinear_obs.push_back(robots[j].get_nonlinear_geometry(time));
        //linear_obs.push_back(robots[j].get_linear_geometry(time));
    }
    robots[i].set_static_obs(external_static_obs);
    robots[i].set_linear_obs(linear_obs);
    robots[i].set_nonlinear_obs(nonlinear_obs);
    poly_obs.push_back(robots[i].get_nonlinear_geometry(time).poly(0));
  }
  for(const auto& it: external_static_obs)
    poly_obs.push_back(it.poly(0));
  return poly_obs;
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "test");
  ros::NodeHandle nh("~");

  std::string file_name;
  std::string states_name, polys_name, paths_name, prs_name;
  nh.param("file", file_name, std::string("sim.bag"));
  nh.param("states_name", states_name, std::string("/states"));
  nh.param("polys_name", polys_name, std::string("/polyhedrons"));
  nh.param("paths_name", paths_name, std::string("/paths"));
  nh.param("prs_name", prs_name, std::string("/prs"));

  ros::Publisher poly_pub =
    nh.advertise<decomp_ros_msgs::PolyhedronArray>(polys_name, 1, true);
  ros::Publisher bound_pub =
    nh.advertise<decomp_ros_msgs::PolyhedronArray>("bound", 1, true);
  ros::Publisher cloud_pub =
      nh.advertise<sensor_msgs::PointCloud>(states_name, 1, true);
  ros::Publisher prs_pub =
      nh.advertise<planning_ros_msgs::PrimitiveArray>(prs_name, 1, true);
  ros::Publisher path_pub =
      nh.advertise<planning_ros_msgs::PathArray>(paths_name, 1, true);

  Polyhedron2D rec;
  rec.add(Hyperplane2D(Vec2f(-0.5, 0), -Vec2f::UnitX()));
  rec.add(Hyperplane2D(Vec2f(0.5, 0), Vec2f::UnitX()));
  rec.add(Hyperplane2D(Vec2f(0, -0.5), -Vec2f::UnitY()));
  rec.add(Hyperplane2D(Vec2f(0, 0.5), Vec2f::UnitY()));

  Vec2f origin, dim;
  nh.param("origin_x", origin(0), 0.0);
  nh.param("origin_y", origin(1), -5.0);
  nh.param("range_x", dim(0), 10.0);
  nh.param("range_y", dim(1), 10.0);

  // Initialize planner
  double dt, v_max, a_max, t_max;
  double u;
  int num;
  nh.param("dt", dt, 1.0);
  nh.param("v_max", v_max, -1.0);
  nh.param("a_max", a_max, -1.0);
  nh.param("u", u, 1.0);
  nh.param("num", num, 1);

  // Set input control
  vec_E<VecDf> U;
  const decimal_t du = u / num;
  for (decimal_t dx = -u; dx <= u; dx += du)
    for (decimal_t dy = -u; dy <= u; dy += du)
      U.push_back(Vec2f(dx, dy));

  Robot<2> robot1(rec);
  robot1.set_v_max(v_max);
  robot1.set_a_max(a_max);
  robot1.set_u(U);
  robot1.set_dt(dt);
  robot1.set_map(origin, dim);
  robot1.set_start(Vec2f(0, -5));
  robot1.set_goal(Vec2f(10, 5));
  robot1.plan(0);

  Robot<2> robot2(rec);
  robot2.set_v_max(v_max);
  robot2.set_a_max(a_max);
  robot2.set_u(U);
  robot2.set_dt(dt);
  robot2.set_map(origin, dim);
  robot2.set_start(Vec2f(0, 0));
  robot2.set_goal(Vec2f(10, 0));
  robot2.plan(0.01);

  Robot<2> robot3(rec);
  robot3.set_v_max(v_max);
  robot3.set_a_max(a_max);
  robot3.set_u(U);
  robot3.set_dt(dt);
  robot3.set_map(origin, dim);
  robot3.set_start(Vec2f(0, 5));
  robot3.set_goal(Vec2f(10, -5));
  robot3.plan(0.02);

  Robot<2> robot4(rec);
  robot4.set_v_max(v_max);
  robot4.set_a_max(a_max);
  robot4.set_u(U);
  robot4.set_dt(dt);
  robot4.set_map(origin, dim);
  robot4.set_start(Vec2f(5, 5));
  robot4.set_goal(Vec2f(5, -5));
  robot4.plan(0.03);

  Robot<2> robot5(rec);
  robot5.set_v_max(v_max);
  robot5.set_a_max(a_max);
  robot5.set_u(U);
  robot5.set_dt(dt);
  robot5.set_map(origin, dim);
  robot5.set_start(Vec2f(10, 5));
  robot5.set_goal(Vec2f(0, -5));
  robot5.plan(0.04);

  Robot<2> robot6(rec);
  robot6.set_v_max(v_max);
  robot6.set_a_max(a_max);
  robot6.set_u(U);
  robot6.set_dt(dt);
  robot6.set_map(origin, dim);
  robot6.set_start(Vec2f(10, 0));
  robot6.set_goal(Vec2f(0, 0));
  robot6.plan(0.05);


  Robot<2> robot7(rec);
  robot7.set_v_max(v_max);
  robot7.set_a_max(a_max);
  robot7.set_u(U);
  robot7.set_dt(dt);
  robot7.set_map(origin, dim);
  robot7.set_start(Vec2f(10, -5));
  robot7.set_goal(Vec2f(0, 5));
  robot7.plan(0.06);


  Robot<2> robot8(rec);
  robot8.set_v_max(v_max);
  robot8.set_a_max(a_max);
  robot8.set_u(U);
  robot8.set_dt(dt);
  robot8.set_map(origin, dim);
  robot8.set_start(Vec2f(5, -5));
  robot8.set_goal(Vec2f(5, 5));
  robot8.plan(0.07);

  vec_E<Robot<2>> robots;
  robots.push_back(robot1);
  robots.push_back(robot2);
  robots.push_back(robot3);
  robots.push_back(robot4);
  robots.push_back(robot5);
  robots.push_back(robot6);
  robots.push_back(robot7);
  robots.push_back(robot8);

  vec_E<PolyhedronObstacle2D> static_obs;
  Polyhedron2D rec1;
  rec1.add(Hyperplane2D(Vec2f(4, 0), -Vec2f::UnitX()));
  rec1.add(Hyperplane2D(Vec2f(6, 0), Vec2f::UnitX()));
  rec1.add(Hyperplane2D(Vec2f(5, -1), -Vec2f::UnitY()));
  rec1.add(Hyperplane2D(Vec2f(5, 1), Vec2f::UnitY()));
  static_obs.push_back(PolyhedronObstacle2D(rec1, Vec2f::Zero()));

  vec_E<Polyhedron2D> bbox;
  bbox.push_back(robots.front().get_bbox());
  decomp_ros_msgs::PolyhedronArray bbox_msg = DecompROS::polyhedron_array_to_ros(bbox);
  bbox_msg.header.frame_id = "map";
  bbox_msg.header.stamp = ros::Time::now();
  bound_pub.publish(bbox_msg);

  ros::Rate loop_rate(100);
  decimal_t update_t = 0.01;
  decimal_t time = 0;
  ros::Time t0 = ros::Time::now();

  std::vector<sensor_msgs::PointCloud> cloud_msgs;
  std::vector<decomp_ros_msgs::PolyhedronArray> poly_msgs;
  std::vector<planning_ros_msgs::PathArray> path_msgs;
  std::vector<planning_ros_msgs::PrimitiveArray> prs_msgs;
  while (ros::ok()) {
    time += update_t;
    std::vector<decimal_t> ts;

    auto poly_obs = set_obs(robots, time, static_obs);

    for(auto& it: robots)
      it.plan(time);

    for(auto& it: poly_obs) {
      for(auto& itt: it.vs_)
        itt.p_ -= itt.n_ * 0.25;
    }

    decomp_ros_msgs::PolyhedronArray poly_msg = DecompROS::polyhedron_array_to_ros(poly_obs);
    poly_msg.header.frame_id = "map";
    poly_msg.header.stamp = t0 + ros::Duration(time);
    poly_pub.publish(poly_msg);
    poly_msgs.push_back(poly_msg);

    vec_E<vec_Vec3f> path_array;
    vec_E<Primitive2D> prs_array;
    vec_Vec2f states;
    for(auto& it: robots) {
      states.push_back(it.get_state(time).pos);
      auto prs = it.get_primitives();
      prs_array.insert(prs_array.end(), prs.begin(), prs.end());
      path_array.push_back(vec2_to_vec3(it.get_history()));
    }

    auto path_msg = path_array_to_ros(path_array);
    path_msg.header.frame_id = "map";
    path_msg.header.stamp = t0 + ros::Duration(time);
    path_pub.publish(path_msg);
    path_msgs.push_back(path_msg);

    auto prs_msg = toPrimitiveArrayROSMsg(prs_array);
    prs_msg.header.frame_id = "map";
    prs_msg.header.stamp = t0 + ros::Duration(time);
    prs_pub.publish(prs_msg);
    prs_msgs.push_back(prs_msg);

    auto cloud_msg = vec_to_cloud(vec2_to_vec3(states));
    cloud_msg.header.frame_id = "map";
    cloud_msg.header.stamp = t0 + ros::Duration(time);
    cloud_pub.publish(cloud_msg);
    cloud_msgs.push_back(cloud_msg);

    ros::spinOnce();

    loop_rate.sleep();
  }

  rosbag::Bag bag;
  bag.open(file_name, rosbag::bagmode::Write);

  for(const auto& it: cloud_msgs)
    bag.write(states_name, it.header.stamp, it);
  for(const auto& it: poly_msgs)
    bag.write(polys_name, it.header.stamp, it);
  for(const auto& it: path_msgs)
    bag.write(paths_name, it.header.stamp, it);
  for(const auto& it: prs_msgs)
    bag.write(prs_name, it.header.stamp, it);

  bag.close();

  /*
  write_bag<sensor_msgs::PointCloud>(file_name, states_name, cloud_msgs);
  write_bag<decomp_ros_msgs::PolyhedronArray>(file_name, polys_name, poly_msgs);
  write_bag<planning_ros_msgs::PathArray>(file_name, paths_name, path_msgs);
  write_bag<planning_ros_msgs::PrimitiveArray>(file_name, prs_name, prs_msgs);
  */

  return 0;
}
