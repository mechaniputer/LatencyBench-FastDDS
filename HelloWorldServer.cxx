// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * @file HelloWorldServer.cpp
 * This file contains the implementation of the subscriber functions.
 *
 * This file was generated by the tool fastddsgen.
 */

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/qos/PublisherQos.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>
#include <fastdds/rtps/transport/TCPv4TransportDescriptor.h>

// For IPLocator
#include <fastdds/rtps/transport/TCPv4TransportDescriptor.h>

#include "HelloWorldServer.h"
#include "HelloWorldPubSubTypes.h"

using namespace eprosima::fastdds::dds;
// For IPLocator
using namespace eprosima::fastrtps::rtps;

HelloWorldServer::HelloWorldServer()
    : participant_(nullptr)
    , subscriber_(nullptr)
    , topic_(nullptr)
    , reader_(nullptr)
    , type_(new HelloWorldPubSubType())
{
}

HelloWorldServer::~HelloWorldServer()
{
    if (reader_ != nullptr)
    {
        subscriber_->delete_datareader(reader_);
    }
    if (topic_ != nullptr)
    {
        participant_->delete_topic(topic_);
    }
    if (subscriber_ != nullptr)
    {
        participant_->delete_subscriber(subscriber_);
    }
    DomainParticipantFactory::get_instance()->delete_participant(participant_);
}

bool HelloWorldServer::init()
{
    // For CPU measurement
    m_cpu_cores = std::thread::hardware_concurrency();
    m_ticks_to_ns = 1000000000LL / ::sysconf(_SC_CLK_TCK);


	// QOS and initial peers
    DomainParticipantQos pqos;

    //CREATE THE PARTICIPANT
    pqos.name("Participant_sub");
    participant_ = DomainParticipantFactory::get_instance()->create_participant(0, pqos);
    if (participant_ == nullptr)
    {
        return false;
    }

    //REGISTER THE TYPE
    type_.register_type(participant_);

    //CREATE THE SUBSCRIBER
    subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);
    if (subscriber_ == nullptr)
    {
        return false;
    }

    //CREATE THE TOPIC
    topic_ = participant_->create_topic(
        "HelloWorldTopic",
        type_.get_type_name(),
        TOPIC_QOS_DEFAULT);
    if (topic_ == nullptr)
    {
        return false;
    }

    //CREATE THE READER
    DataReaderQos rqos = DATAREADER_QOS_DEFAULT;
    rqos.reliability().kind = RELIABLE_RELIABILITY_QOS;
    reader_ = subscriber_->create_datareader(topic_, rqos, &listener_);
    if (reader_ == nullptr)
    {
        return false;
    }


    //CREATE THE RESPONSE PUBLISHER
    response_publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
    if (response_publisher_ == nullptr)
    {
        return false;
    }
    //CREATE THE RESPONSE TOPIC
    response_topic_ = participant_->create_topic(
        "ResponseTopic",
        type_.get_type_name(),
        TOPIC_QOS_DEFAULT);
    if (response_topic_ == nullptr)
    {
        return false;
    }
    // CREATE THE RESPONSE WRITER
	DataWriterQos response_qos;
	response_qos.reliable_writer_qos().times.heartbeatPeriod.seconds = 0;
	response_qos.reliable_writer_qos().times.heartbeatPeriod.nanosec = 50000;
    response_writer_ = response_publisher_->create_datawriter(response_topic_, response_qos, &response_listener_);
    if (response_writer_ == nullptr)
    {
        return false;
    }

    return true;
}

void HelloWorldServer::run()
{
    std::cout << "Server waiting for Listener." << std::endl;
    while (listener_.matched == 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(250)); // Sleep 250 ms
    }
	std::cout << "Matched.\n";

    HelloWorld st;
    SampleInfo info;
    std::cout << "Waiting for Data. " << std::endl;
    while(true){
		std::cout << "Waiting\n";
        if(reader_->wait_for_unread_message(10)) {
			std::cout << "Taking\n";
            if (reader_->take_next_sample(&st, &info) == ReturnCode_t::RETCODE_OK) {
                if (info.valid_data) {
					std::cout << "Responding\n";
                    // Print your structure data here.
                    //++(listener_.samples);
                    //std::cout << "Sample received, count=" << listener_.samples << std::endl;
                    response_writer_->write(&st);
                    //std::cout << "Response sent back\n";
                }
            }
        }else{
            std::cout << "Wait timed out\n";
            break;
        }
    }
    std::cout << "Shutting down the Server." << std::endl;
}
