/**************************************************************
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 *************************************************************/



#ifndef DOCUMENT_STATISTIC_HXX_INCLUDED
#define DOCUMENT_STATISTIC_HXX_INCLUDED

#include <utility>
#include <string>
#include <vector>
#include "internal/metainforeader.hxx"


//------------------------------------
//
//------------------------------------

struct statistic_item
{
	statistic_item();

	statistic_item(
		const std::wstring& title,
		const std::wstring& value,
		bool editable) :
		title_(title),
		value_(value),
		editable_(editable)
	{}

	std::wstring title_;
	std::wstring value_;
	bool editable_;
};

//------------------------------------
//
//------------------------------------

typedef std::vector<statistic_item>                     statistic_item_list_t;
typedef std::pair<std::wstring, statistic_item_list_t>  statistic_group_t;
typedef std::vector<statistic_group_t>                  statistic_group_list_t;

//------------------------------------
//
//------------------------------------

class document_statistic_reader;
typedef std::auto_ptr<document_statistic_reader> document_statistic_reader_ptr;

document_statistic_reader_ptr create_document_statistic_reader(const std::string& document_name, CMetaInfoReader* meta_info_accessor);

//------------------------------------
//
//------------------------------------

class document_statistic_reader
{
public:
	virtual ~document_statistic_reader();

	void read(statistic_group_list_t* group_list);

	std::string get_document_name() const;

protected:
	document_statistic_reader(const std::string& document_name, CMetaInfoReader* meta_info_accessor);

	virtual void fill_description_section(CMetaInfoReader *meta_info_accessor,statistic_group_list_t* group_list) = 0;

	virtual void fill_origin_section( CMetaInfoReader *meta_info_accessor,statistic_group_list_t* group_list);

private:
	std::string document_name_;
	CMetaInfoReader* meta_info_accessor_;

	friend document_statistic_reader_ptr create_document_statistic_reader(
		const std::string& document_name, CMetaInfoReader* meta_info_accessor);
};

//------------------------------------
//
//------------------------------------

class writer_document_statistic_reader : public document_statistic_reader
{
protected:
	writer_document_statistic_reader(const std::string& document_name, CMetaInfoReader* meta_info_accessor);

	virtual void fill_description_section(CMetaInfoReader *meta_info_accessor, statistic_group_list_t* group_list);

	friend document_statistic_reader_ptr create_document_statistic_reader(
		const std::string& document_name, CMetaInfoReader* meta_info_accessor);
};

//------------------------------------
//
//------------------------------------

class calc_document_statistic_reader : public document_statistic_reader
{
protected:
	calc_document_statistic_reader(const std::string& document_name, CMetaInfoReader* meta_info_accessor);

	virtual void fill_description_section( CMetaInfoReader *meta_info_accessor,statistic_group_list_t* group_list);

	friend document_statistic_reader_ptr create_document_statistic_reader(
		const std::string& document_name, CMetaInfoReader* meta_info_accessor);
};

//------------------------------------
//
//------------------------------------

class draw_impress_math_document_statistic_reader : public document_statistic_reader
{
protected:
	draw_impress_math_document_statistic_reader(const std::string& document_name, CMetaInfoReader* meta_info_accessor);

	virtual void fill_description_section(CMetaInfoReader *meta_info_accessor, statistic_group_list_t* group_list);

	friend document_statistic_reader_ptr create_document_statistic_reader(
		const std::string& document_name, CMetaInfoReader* meta_info_accessor);
};

#endif

/* vim: set noet sw=4 ts=4: */
