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
 * @file HelloWorldListener.cpp
 * This file contains the implementation of the publisher functions.
 *
 * This file was generated by the tool fastddsgen.
 */


#include "HelloWorldListener.h"
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

HelloWorldListener::HelloWorldListener()
	: participant_(nullptr)
	, publisher_(nullptr)
	, topic_(nullptr)
	, writer_(nullptr)
	, type_(new HelloWorldPubSubType())
{
}

HelloWorldListener::~HelloWorldListener()
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

bool HelloWorldListener::init(){

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

	//CREATE THE TOPIC
	topic_ = participant_->create_topic(
		"HelloWorldTopic",
		type_.get_type_name(),
		TOPIC_QOS_DEFAULT);
	if (topic_ == nullptr)
	{
		return false;
	}

	//CREATE THE RESPONSE SUBSCRIBER
	response_subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);
	if (response_subscriber_ == nullptr)
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
	//CREATE THE RESPONSE READER
	DataReaderQos rqos = DATAREADER_QOS_DEFAULT;
	rqos.reliability().kind = RELIABLE_RELIABILITY_QOS;
	rqos.history().kind = KEEP_ALL_HISTORY_QOS;
	rqos.durability().kind = TRANSIENT_LOCAL_DURABILITY_QOS;
	response_reader_ = response_subscriber_->create_datareader(response_topic_, rqos, &response_listener_);
	if (response_reader_ == nullptr)
	{
		return false;
	}

	std::cout << "Listener entities created." << std::endl;
	return true;
}

void HelloWorldListener::PubListener::on_publication_matched(
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

void HelloWorldListener::run() {
	HelloWorld st;
	SampleInfo info;
	unsigned long long num_samples = 0;

	while(true){
		std::cout << "Waiting for initial sample. " << std::endl;
		if(response_reader_->wait_for_unread_message(10)) {
			if (response_reader_->take_next_sample(&st, &info) == ReturnCode_t::RETCODE_OK) {
				if (info.valid_data) {
					num_samples = std::stoull(st.message());
					std::cout << "Expecting " << num_samples << " samples.\n";
					break;
				}
			}
		}else{
			std::cout << "Wait timed out\n";
			exit(-1);
		}
	}

	uint64_t total_latency = 0;
	unsigned long long rx_count = 0;
	auto time_begin = std::chrono::high_resolution_clock::now();
	// Main loop
	while(rx_count < num_samples){
//		std::cout << "Waiting\n";
		if(response_reader_->wait_for_unread_message(10)) {
//			std::cout << "Taking\n";
			while(rx_count < num_samples){
				if(response_reader_->take_next_sample(&st, &info) != ReturnCode_t::RETCODE_OK) break;

				if (info.valid_data) {
					auto time_rx = std::chrono::high_resolution_clock::now();
					auto time_rx_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(time_rx.time_since_epoch()).count();
					rx_count += 1;
					//std::cout << "Received response " << rx_count << std::endl;;

					// Compute latency of this sample
					int64_t nanoseconds_from_sample;
					std::memcpy(&nanoseconds_from_sample, &st.message()[0], sizeof(int64_t));
					auto time_tx = std::chrono::time_point<std::chrono::high_resolution_clock>(std::chrono::nanoseconds(nanoseconds_from_sample));
					//std::cout << "Timestamp from sample: " << nanoseconds_from_sample << std::endl;
					//std::cout << "Timestamp received: " << time_rx_ns << std::endl;
					//std::cout << "Latency: " << time_rx_ns - nanoseconds_from_sample << " ns\n";
					total_latency += time_rx_ns - nanoseconds_from_sample;
				}
			}
		}else{
			std::cout << "Wait timed out\n";
			exit(-1);
		}
	}

	auto time_end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> running_time = time_end-time_begin;
	std::cout << "Finished receiving " << num_samples << " samples in " << running_time.count() << " seconds.\n";
	unsigned long long data_rate = num_samples / running_time.count();
	std::cout << "Data rate was " << data_rate << " samples/sec.\n";
	unsigned long long avg_latency = total_latency / num_samples;
	std::cout << "Average latency per sample was " << avg_latency << " ns\n";
	double avg_latency_sec = (double)avg_latency / 1000000000;
	std::cout << "That is " << avg_latency_sec << " seconds\n";
	std::cout << "Average samples in flight: " << data_rate*avg_latency_sec << "\n";
}

