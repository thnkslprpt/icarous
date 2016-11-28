/**
 * MAVLink message decoder
 * 
 * This is a class that will decode MAVLink messages
 *
 * Contact: Swee Balachandran (swee.balachandran@nianet.org)
 * 
 * 
 * Copyright (c) 2011-2016 United States Government as represented by
 * the National Aeronautics and Space Administration.  No copyright
 * is claimed in the United States under Title 17, U.S.Code. All Other
 * Rights Reserved.
 *
 * Notices:
 *  Copyright 2016 United States Government as represented by the Administrator of the National Aeronautics and Space Administration. 
 *  All rights reserved.
 *     
 * Disclaimers:
 *  No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF ANY KIND, EITHER EXPRESSED, 
 *  IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY
 *  IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR FREEDOM FROM INFRINGEMENT, 
 *  ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, 
 *  WILL CONFORM TO THE SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN ENDORSEMENT BY GOVERNMENT 
 *  AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS, RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS 
 *  RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY DISCLAIMS ALL WARRANTIES AND 
 *  LIABILITIES REGARDING THIRD-PARTY SOFTWARE, IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
 *
 * Waiver and Indemnity:  
 *   RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE UNITED STATES GOVERNMENT, 
 *   ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE 
 *   RESULTS IN ANY LIABILITIES, DEMANDS, DAMAGES,
 *   EXPENSES OR LOSSES ARISING FROM SUCH USE, INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, 
 *   RECIPIENT'S USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE UNITED STATES GOVERNMENT, 
 *   ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  
 *   RECIPIENT'S SOLE REMEDY FOR ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS AGREEMENT.
 */

 #include "MAVLinkInbox.h"

 MAVLinkInbox::MAVLinkInbox(){
     pthread_mutex_init(&lock, NULL);
 }

 void MAVLinkInbox::DecodeMessage(mavlink_message_t message){
    pthread_mutex_lock(&lock);

    switch (message.msgid){

        case MAVLINK_MSG_ID_HEARTBEAT:
        {
            //printf("MAVLINK_MSG_ID_HEARTBEAT\n");
            mavlink_msg_heartbeat_decode(&message, &heartbeat);
            break;
        }

        case MAVLINK_MSG_ID_MISSION_COUNT:
        {
            mavlink_mission_count_t msg;
            mavlink_msg_mission_count_decode(&message, &msg);
            listMissionCount.push(msg);
            break;
        }

        case MAVLINK_MSG_ID_MISSION_REQUEST_LIST:
        {
            mavlink_mission_request_list_t msg;
            mavlink_msg_mission_request_list_decode(&message, &msg);
            listMissionRequestList.push(msg);
            break;
        }

        case MAVLINK_MSG_ID_PARAM_REQUEST_LIST:
        {
            mavlink_param_request_list_t msg;
            mavlink_msg_param_request_list_decode(&message, &msg);
            listParamRequestList.push(msg);
            break;
        }

        case MAVLINK_MSG_ID_PARAM_REQUEST_READ:
        {
            mavlink_param_request_read_t msg;
            mavlink_msg_param_request_read_decode(&message, &msg);
            listParamRequestRead.push(msg);
            break;
        }

        case MAVLINK_MSG_ID_PARAM_SET:
        {
            mavlink_param_set_t msg;
            mavlink_msg_param_set_decode(&message, &msg);
            listParamSet.push(msg);
            break;
        }

        case MAVLINK_MSG_ID_COMMAND_LONG:
        {
            mavlink_command_long_t msg;
            mavlink_msg_command_long_decode(&message, &msg);
            listCommandLong.push(msg);
            break;
        }

        case MAVLINK_MSG_ID_COMMAND_INT:
        {
            mavlink_command_int_t msg;
            mavlink_msg_command_int_decode(&message, &msg);
            listCommandInt.push(msg);
            break;
        }

        case MAVLINK_MSG_ID_SET_MODE:
        {
            mavlink_set_mode_t msg;
            mavlink_msg_set_mode_decode(&message, &msg);
            listSetMode.push(msg);
            break;
        }


    }
    pthread_mutex_unlock(&lock);
 }