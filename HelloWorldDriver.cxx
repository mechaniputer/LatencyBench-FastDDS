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
 * @file HelloWorldDriver.cpp
 * This file contains the implementation of the publisher functions.
 *
 * This file was generated by the tool fastddsgen.
 */


#include "HelloWorldDriver.h"
#include "HelloWorldPubSubTypes.h"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/qos/PublisherQos.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>

// For IPLocator
#include <fastdds/rtps/transport/TCPv4TransportDescriptor.h>

#include <thread>
#include <chrono>

using namespace eprosima::fastdds::dds;
// For IPLocator
using namespace eprosima::fastrtps::rtps;

HelloWorldDriver::HelloWorldDriver()
    : participant_(nullptr)
    , publisher_(nullptr)
    , topic_(nullptr)
    , writer_(nullptr)
    , type_(new HelloWorldPubSubType())
{
}

HelloWorldDriver::~HelloWorldDriver()
{
    if (writer_ != nullptr)
    {
        publisher_->delete_datawriter(writer_);
    }
    if (publisher_ != nullptr)
    {
        participant_->delete_publisher(publisher_);
    }
    if (topic_ != nullptr)
    {
        participant_->delete_topic(topic_);
    }
    DomainParticipantFactory::get_instance()->delete_participant(participant_);
}

bool HelloWorldDriver::init(unsigned size, unsigned rate){
    /* Initialize data_ here */
    auto period = std::chrono::duration<double>(1.0 / rate);
    auto period_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(period);
    m_time_between_publish = period_ns;
    std::cout << (int)m_time_between_publish.count() << std::endl;

	// QOS and initial peers
    DomainParticipantQos pqos;

    //CREATE THE PARTICIPANT
    pqos.name("Participant_pub");
    participant_ = DomainParticipantFactory::get_instance()->create_participant(0, pqos);
    if (participant_ == nullptr)
    {
        return false;
    }

    //REGISTER THE TYPE
    type_.register_type(participant_);

    //CREATE THE PUBLISHER
    publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
    if (publisher_ == nullptr)
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

    // CREATE THE WRITER
	DataWriterQos driver_qos;
	driver_qos.reliable_writer_qos().times.heartbeatPeriod.seconds = 0;
	driver_qos.reliable_writer_qos().times.heartbeatPeriod.nanosec = 50000;
	driver_qos.history().kind = KEEP_ALL_HISTORY_QOS;
    writer_ = publisher_->create_datawriter(topic_, driver_qos, &listener_);
    if (writer_ == nullptr)
    {
        return false;
    }

    std::cout << "Driver entities created." << std::endl;
    return true;
}

void HelloWorldDriver::PubListener::on_publication_matched(
        eprosima::fastdds::dds::DataWriter*,
        const eprosima::fastdds::dds::PublicationMatchedStatus& info)
{
    if (info.current_count_change == 1)
    {
        matched = info.total_count;
        std::cout << "DataWriter matched." << std::endl;
    }
    else if (info.current_count_change == -1)
    {
        matched = info.total_count;
        std::cout << "DataWriter unmatched." << std::endl;
    }
    else
    {
        std::cout << info.current_count_change
                  << " is not a valid value for PublicationMatchedStatus current count change" << std::endl;
    }
}

void HelloWorldDriver::run(unsigned size, unsigned rate) {
	unsigned long long num_samples = 500000;
    std::cout << "HelloWorld DataWriter waiting for DataReaders." << std::endl;
    while (listener_.matched == 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(250)); // Sleep 250 ms
    }
	std::cout << "Matched.\n";

    std::string s0 ("");
	// size>>3 divides by 8 because the test string is already 8 bytes
    for(int i=0; i<(size>>3); i++){
        s0.append("DEADBEEF");
    }
    HelloWorld st;
    st.message(s0);

    std::cout << "msg len: " << st.message().length() << std::endl;
    /* Initialize your structure here */

	unsigned long long tx_count = 0;

    // For send interval timing
    m_first_run = std::chrono::steady_clock::now();
    auto loopcount = 0;
	auto time_begin = std::chrono::high_resolution_clock::now();
    while(tx_count < num_samples){
        // For latency measurement
        auto start = std::chrono::steady_clock::now().time_since_epoch().count();
        writer_->write(&st);
		tx_count += 1;
        std::cout << "Sent sample, count=" << tx_count << std::endl;

		// Uncomment for data rate limiting. Comment for max throughput test.
        //std::chrono::steady_clock::time_point next_run = m_first_run + m_time_between_publish * loopcount++;
        //std::this_thread::sleep_until(next_run);
    }
	auto time_end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> running_time = time_end-time_begin;
	std::cout << "Finished sending " << num_samples << " samples in " << running_time.count() << " seconds.\n";
}

