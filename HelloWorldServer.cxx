// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//	 http://www.apache.org/licenses/LICENSE-2.0
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
	rqos.history().kind = KEEP_ALL_HISTORY_QOS;
	rqos.durability().kind = TRANSIENT_LOCAL_DURABILITY_QOS;
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
	response_qos.history().kind = KEEP_ALL_HISTORY_QOS;
	response_qos.durability().kind = TRANSIENT_LOCAL_DURABILITY_QOS;
	response_writer_ = response_publisher_->create_datawriter(response_topic_, response_qos, &response_listener_);
	if (response_writer_ == nullptr)
	{
		return false;
	}

	return true;
}

void HelloWorldServer::run()
{
	HelloWorld st;
	SampleInfo info;
	unsigned long long num_samples = 0;

	while(true){
		std::cout << "Waiting for initial sample. " << std::endl;
		if(reader_->wait_for_unread_message(10)) {
			if (reader_->take_next_sample(&st, &info) == ReturnCode_t::RETCODE_OK) {
				if (info.valid_data) {
					num_samples = std::stoull(st.message());
					std::cout << "Expecting " << num_samples << " samples. Passing it on.\n";
					response_writer_->write(&st);
					break;
				}
			}
		}else{
			std::cout << "Wait timed out\n";
			exit(-1);
		}
	}

	// Begin CPU measurement
	unsigned int cpu_cores = std::thread::hardware_concurrency();
	int64_t ticks_to_ns = 1000000000LL / ::sysconf(_SC_CLK_TCK);
	auto wall_time_start = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	tms tm;
	::times(&tm);
	auto system_time_start = (tm.tms_stime + tm.tms_cstime) * ticks_to_ns;
	auto user_time_start = (tm.tms_utime + tm.tms_cutime) * ticks_to_ns;

	// Main loop
	unsigned long long tx_count = 0;
	while(tx_count < num_samples){
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
					tx_count += 1;
					//std::cout << "Response sent back\n";
				}
			}
		}else{
			std::cout << "Wait timed out\n";
			break;
		}
	}
	std::cout << "Finished forwarding " << num_samples << " samples\n";

	// End CPU measurement
	auto wall_time_end = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	::times(&tm);
	auto system_time_end = (tm.tms_stime + tm.tms_cstime) * ticks_to_ns;
	auto user_time_end = (tm.tms_utime + tm.tms_cutime) * ticks_to_ns;
	auto wall_diff = wall_time_end - wall_time_start;
	auto system_diff = system_time_end - system_time_start;
	auto user_diff = user_time_end - user_time_start;
	std::cout << "user_diff="<< user_diff << "\nsystem_diff=" << system_diff << "\n";
	std::cout << "CPU: " << 100.0F * static_cast<float>(user_diff + system_diff) / static_cast<float>(wall_diff * cpu_cores) << "%\n";

}
