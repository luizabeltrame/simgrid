/* Copyright (c) 2010-2014. The SimGrid Team.
 * All rights reserved.                                                     */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#include <stdio.h>
#include "msg/msg.h"            /* Yeah! If you want to use msg, you need to include msg/msg.h */
#include "xbt/sysdep.h"         /* calloc, printf */

/* Create a log channel to have nice outputs. */
#include "xbt/log.h"
#include "xbt/asserts.h"
XBT_LOG_NEW_DEFAULT_CATEGORY(msg_test,
                             "Messages specific for this msg example");

int test(int argc, char *argv[]);
int daemon(int argc, char *argv[]);
int commRX(int argc, char *argv[]);
int commTX(int argc, char *argv[]);

xbt_dynar_t tests;

int test(int argc, char *argv[])
{
  int test = 0;
  char **argvF;
  argvF = xbt_new(char*, 2);
  argvF[0] = xbt_strdup("daemon");
  msg_host_t jupiter = MSG_get_host_by_name("Jupiter");

  test = 1;
  if (xbt_dynar_member(tests, &test)){
    XBT_INFO("Test 1:");
    XBT_INFO("  Create a process on Jupiter");
    argvF = xbt_new(char*, 2);
    argvF[0] = xbt_strdup("daemon");
    MSG_process_create_with_arguments("daemon", daemon, NULL, jupiter, 1, argvF);
    XBT_INFO("  Turn off Jupiter");
    MSG_host_off(jupiter);
    MSG_process_sleep(10);
    XBT_INFO("Test 1 seems ok, cool !(number of Process : %d, it should be 1 (i.e. the Test one))", MSG_process_get_number());
  }

  test = 2;
  if (xbt_dynar_member(tests, &test)){
    XBT_INFO("Test 2:");
    XBT_INFO("  Turn off Jupiter");
    MSG_host_off(jupiter);
    argvF = xbt_new(char*, 2);
    argvF[0] = xbt_strdup("daemon");
    MSG_process_create_with_arguments("daemon", daemon, NULL, jupiter, 1, argvF);
    MSG_process_sleep(10);
    XBT_INFO("  Test 2 does not crash, WTF ?!(number of Process : %d, it should be 1)", MSG_process_get_number());
    XBT_INFO("  Ok so let's turn on/off the node to see whether the process is correctly bound to Jupiter");
    MSG_host_on(jupiter);
    XBT_INFO("  Turn off");
    MSG_host_off(jupiter);
    XBT_INFO("  sleep");
    MSG_process_sleep(10);
    XBT_INFO("number of Process : %d it should be 1. The daemon that has been created for test2 has been correctly destroyed....ok at least it looks rigorous, cool ! You just have to disallow the possibility to create a new process on a node when the node is off.)", MSG_process_get_number());
  }

  test = 3;
  if (xbt_dynar_member(tests, &test)){
    XBT_INFO("Test 3 (turn off src during a communication) : Create a Process/task to make a communication between Jupiter and Tremblay and turn off Jupiter during the communication");
    MSG_host_on(jupiter);
    MSG_process_sleep(10);
    argvF = xbt_new(char*, 2);
    argvF[0] = xbt_strdup("commRX");
    MSG_process_create_with_arguments("commRX", commRX, NULL, MSG_get_host_by_name("Tremblay"), 1, argvF);
    argvF = xbt_new(char*, 2);
    argvF[0] = xbt_strdup("commTX");
    MSG_process_create_with_arguments("commTX", commTX, NULL, MSG_get_host_by_name("Jupiter"), 1, argvF);
    XBT_INFO("  number of processes: %d", MSG_process_get_number());
    MSG_process_sleep(10);
    XBT_INFO("  Turn Jupiter off");
    MSG_host_off(jupiter);
    XBT_INFO("Test 3 seems ok  (number of Process : %d, it should be 1 or 2 if RX has not been satisfied) cool, you can now turn off a node that has a process paused by a sleep call", MSG_process_get_number());
  }

  test = 4;
  if (xbt_dynar_member(tests, &test)){
    XBT_INFO("Test 4 (turn off dest during a communication : Create a Process/task to make a communication between Tremblay and Jupiter and turn off Jupiter during the communication");
    MSG_host_on(jupiter);
    MSG_process_sleep(10);
    argvF = xbt_new(char*, 2);
    argvF[0] = xbt_strdup("commTX");
    MSG_process_create_with_arguments("commTX", commTX, NULL, MSG_get_host_by_name("Tremblay"), 1, argvF);
    argvF = xbt_new(char*, 2);
    argvF[0] = xbt_strdup("commRX");
    MSG_process_create_with_arguments("commRX", commRX, NULL, MSG_get_host_by_name("Jupiter"), 1, argvF);
    MSG_host_off(jupiter);
    XBT_INFO("Test 4 seems ok, cool !(number of Process : %d, it should be 1", MSG_process_get_number());
  }

  test =5;
  if (xbt_dynar_member(tests, &test)){

  }

  test = 6;
  if (xbt_dynar_member(tests, &test)){

  }

  test = 7;
  if (xbt_dynar_member(tests, &test)){

  }

  test = 8;
  if (xbt_dynar_member(tests, &test)){

  }

  return 0;
}

int daemon(int argc, char *argv[])
{
  msg_task_t task = NULL;
  task = MSG_task_create("deamon", 100*MSG_get_host_speed(MSG_host_self()), 0, NULL);
  XBT_INFO("  Execute deamon");
  MSG_task_execute(task);
  XBT_INFO("  I'm done. See you!");
  return 0;
}

int commTX(int argc, char *argv[])
{
  msg_task_t task = NULL;
  char mailbox[80];
  sprintf(mailbox, "comm");
  XBT_INFO("  Start TX");
  task = MSG_task_create("COMM", 0, 100000000, NULL);
  MSG_task_isend(task, mailbox);
  // We should wait a bit (if not the process will end before the communication, leading to an exception at the other side).
  MSG_process_sleep(30);
  XBT_INFO("  TX done");
  return 0;
}

int commRX(int argc, char *argv[])
{
  msg_task_t task = NULL;
  char mailbox[80];
  sprintf(mailbox, "comm");
  XBT_INFO("  Start RX");
  msg_error_t error = MSG_task_receive(&(task), mailbox);
  if (error==MSG_OK) {
    XBT_INFO("  Receive message: %s", task->name);
  } else if (error==MSG_HOST_FAILURE) {
    XBT_INFO("  Receive message: HOST_FAILURE");
  } else if (error==MSG_TRANSFER_FAILURE) {
    XBT_INFO("  Receive message: TRANSFERT_FAILURE");
  } else {
    XBT_INFO("  Receive message: %d", error);
  }
  XBT_INFO("  RX Done");
  return 0;
}

/** Main function */
int main(int argc, char *argv[])
{
  msg_error_t res;
  const char *platform_file;
  const char *application_file;

  MSG_init(&argc, argv);
  if (argc != 4) {
    printf("Usage: %s platform_file deployment_file test_number\n", argv[0]);
    printf("example: %s msg_platform.xml msg_deployment.xml 1\n", argv[0]);
    exit(1);
  }

  unsigned int iter;
  char *groups;
  xbt_dynar_t s_tests = xbt_str_split(argv[3], ",");
  int tmp_test = 0;
  tests = xbt_dynar_new(sizeof(int), NULL);
  xbt_dynar_foreach(s_tests, iter, groups) {
     sscanf(xbt_dynar_get_as(s_tests, iter, char *), "%d", &tmp_test);
     xbt_dynar_set_as(tests, iter, int, tmp_test);
  }

  platform_file = argv[1];
  application_file = argv[2];

  /* MSG_config("workstation/model","KCCFLN05"); */
  {                             /*  Simulation setting */
    MSG_create_environment(platform_file);
  }
  {                             /*   Application deployment */
    MSG_function_register("test", test);
    MSG_function_register("daemon", daemon);

    MSG_launch_application(application_file);
  }
  res = MSG_main();

  XBT_INFO("Simulation time %g", MSG_get_clock());

  if (res == MSG_OK)
    return 0;
  else
    return 1;
}                               /* end_of_main */