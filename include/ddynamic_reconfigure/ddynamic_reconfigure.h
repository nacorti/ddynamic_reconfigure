///////////////////////////////////////////////////////////////////////////////

// Copyright (C) 2014, 2015 PAL Robotics S.L.

// All rights reserved.

//////////////////////////////////////////////////////////////////////////////

// Author Hilario Tomé

#ifndef _DDYNAMIC_RECONFIGURE_
#define _DDYNAMIC_RECONFIGURE_

#include <dynamic_reconfigure/server.h>
#include <ddynamic_reconfigure/registered_param.h>
#include <ros/ros.h>
#include <ros/callback_queue.h>
#include <ros/spinner.h>

namespace ddynamic_reconfigure
{

/**
 * @brief The DDynamicReconfigure class allows to use ROS dynamic reconfigure without the
 * need to write
 * a custom cpf file, variables are registered and exposed at run time. 
 * Modification of the variables is done through a variable pointer or through a callback function.
 */
class DDynamicReconfigure
{
public:
  /**
    * @param nh the queue associated to this nh should spined() somewhere else
    */
  DDynamicReconfigure(const ros::NodeHandle &nh = ros::NodeHandle("~"));

  virtual ~DDynamicReconfigure();

  /**
   * @brief registerVariable register a variable to be modified via the
   * dynamic_reconfigure API. When a change is made, it will be reflected in the
   * variable directly
   */
  void registerVariable(const std::string &name, int *variable,
                        const std::string &description = "", int min = -100, int max = 100);

  void registerVariable(const std::string &name, double *variable,
                        const std::string &description = "", double min = -100,
                        double max = 100);

  void registerVariable(const std::string &name, bool *variable,
                        const std::string &description = "");


  void registerEnumVariable(const std::string &name, int *variable,
                            const std::string &description = "",
                            std::map<std::string, int> enum_dict = {},
                            const std::string &enum_description = "");
  void registerEnumVariable(const std::string &name, double *variable,
                            const std::string &description = "",
                            std::map<std::string, double> enum_dict = {},
                            const std::string &enum_description = "");


  /**
   * @brief registerVariable register a variable to be modified via the
   * dynamic_reconfigure API. When a change is made, the callback will be called with the
   * new value
   */
  void registerVariable(const std::string &name, int current_value,
                        const boost::function<void(int value)> &callback,
                        const std::string &description = "", int min = -100, int max = 100);
  void registerVariable(const std::string &name, double current_value,
                        const boost::function<void(double value)> &callback,
                        const std::string &description = "", double min = -100,
                        double max = 100);
  void registerVariable(const std::string &name, bool current_value,
                        const boost::function<void(bool value)> &callback,
                        const std::string &description = "");

  void registerEnumVariable(const std::string &name, int current_value,
                            const boost::function<void(int)> &callback,
                            const std::string &description,
                            std::map<std::string, int> enum_dict = {},
                            const std::string &enum_description = "");

  void registerEnumVariable(const std::string &name, double current_value,
                            const boost::function<void(double)> &callback,
                            const std::string &description,
                            std::map<std::string, double> enum_dict = {},
                            const std::string &enum_description = "");
  /**
   * @brief publishServicesTopics starts the server once all the needed variables are
   * registered
   */
  void publishServicesTopics();

  void updatePublishedInformation();

  typedef boost::function<void()> UserCallbackType;
  
  /**
   * @brief setUserCallback An optional callback that will be called whenever a value is changed
   */
  void setUserCallback(const UserCallbackType &callback);

  void clearUserCallback();

  
  /**
   * Deprecated functions. For backwards compatibility
   */
  template <typename T>
  void RegisterVariable(T *variable, std::string id, double min = -100, double max = 100)
  {
    registerVariable(id, variable, "", min, max);
  }
  
  void RegisterVariable(bool *variable, std::string id)
  {
    registerVariable(id, variable, "");
  }
  
  void PublishServicesTopics();

private:
  dynamic_reconfigure::ConfigDescription generateConfigDescription() const;

  dynamic_reconfigure::Config generateConfig();

  bool setConfigCallback(dynamic_reconfigure::Reconfigure::Request &req,
                         dynamic_reconfigure::Reconfigure::Response &rsp);

  /**
   * @brief setUserCallback Set a function to be called when parameters have changed
   */

  ros::NodeHandle node_handle_;
  ros::ServiceServer set_service_;
  ros::Publisher update_pub_;
  ros::Publisher descr_pub_;

  bool advertized_;

  // Registered variables
  std::vector<std::unique_ptr<RegisteredParam<int>>> registered_int_;
  std::vector<std::unique_ptr<RegisteredParam<double>>> registered_double_;
  std::vector<std::unique_ptr<RegisteredParam<bool>>> registered_bool_;
  
  UserCallbackType user_callback_;
  
  ros::Timer pub_config_timer_;
  dynamic_reconfigure::Config last_config_;
};

typedef boost::shared_ptr<DDynamicReconfigure> DDynamicReconfigurePtr;
}

#endif
