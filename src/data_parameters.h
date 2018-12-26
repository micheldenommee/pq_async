/*
    This file is part of libpq-async++
    Copyright (C) 2011-2018 Michel Denommee (and other contributing authors)
    
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef _libpq_async_parameters_h
#define _libpq_async_parameters_h

#include "stable_headers.h"
#include "exceptions.h"
#include "utils.h"

#include "pg_types.h"

#include <boost/multi_array.hpp>

extern "C" {
#include INCLUDE_FILE(LIBPQ_POSTGRESQL_INCLUDE_DIR,libpq-fe.h)
#include "../deps/pg_type_d.h"
}

#define PG_BIN_FORMAT 1
#define PG_TXT_FORMAT 0

/*
TODO: look if we should replace defined OID with fetched ones.

https://www.postgresql.org/docs/9.2/catalog-pg-type.html

select t.oid, t.typname, t.typarray, t.typtype, t.typcategory, 
    n.nspname as "schema", 
    pg_catalog.format_type(t.oid, NULL) AS "name",
    pg_catalog.obj_description(t.oid, 'pg_type') as "desc"
from pg_type t
    left join pg_catalog.pg_namespace n ON n.oid = t.typnamespace;


select t.oid, t.typname, t.typarray, t.typtype, t.typcategory, 
    n.nspname as "schema", 
    pg_catalog.format_type(t.oid, NULL) AS "name"
from pg_type t
    left join pg_catalog.pg_namespace n ON n.oid = t.typnamespace;
*/

namespace pq_async{

/*!
 * \brief 
 * 
 */
class parameter
{
public:
    /*!
     * \brief Construct a new parameter object
     * 
     * \param oid PostgreSQL OID value.
     * \param value network byte order serialized value
     * \param length length of the value param
     * \param format serialized value format (0 for text, 1 for bin)
     */
    parameter(Oid oid, char* value, int length, int format)
        : _oid(oid), _has_oid(true), _value(value), 
        _length(length), _format(format)
    {
    }
    
    /*!
     * \brief Construct a new parameter object
     * 
     * \param value network byte order serialized value
     * \param length length of the value param
     * \param format serialized value format (0 for text, 1 for bin)
     */
    parameter(char* value, int length, int format)
        :_oid(0), _has_oid(false), _value(value), 
        _length(length), _format(format)
    {
    }
    
    /*!
     * \brief Destroy the parameter object
     * 
     */
    ~parameter()
    {
        if(_value)
            delete[] _value;
    }
    
    Oid get_oid() const { return _oid;}
    bool has_oid() const { return _has_oid;}
    
    const char* get_value() const { return _value;}
    int get_length() const { return _length;}
    int get_format() const { return _format;}

private:
    Oid _oid;
    bool _has_oid;
    char* _value;
    int _length;
    int _format;
};


// =============================================
// val_from_pgparam...==========================
// =============================================

template < typename T >
T val_from_pgparam(int oid, char* val, int len, int format)
{
    std::string errMsg = "Convertion from postgres to: \"";
    errMsg.append(typeid(T).name() );
    errMsg.append("\" is not supported.");
    throw pq_async::exception(errMsg);
}

template<>
bool val_from_pgparam<bool>(int oid, char* val, int len, int format);

template<>
std::string val_from_pgparam<std::string>(
    int oid, char* val, int len, int format
);

template<>
int16_t val_from_pgparam<int16_t>(int oid, char* val, int len, int format);

template<>
int32_t val_from_pgparam<int32_t>(int oid, char* val, int len, int format);

template<>
int64_t val_from_pgparam<int64_t>(int oid, char* val, int len, int format);

template<>
float val_from_pgparam<float>(int oid, char* val, int len, int format);

template<>
double val_from_pgparam<double>(int oid, char* val, int len, int format);

template<>
pq_async::time val_from_pgparam<pq_async::time>(
    int oid, char* val, int len, int format
);
template<>
pq_async::time_tz val_from_pgparam<pq_async::time_tz>(
    int oid, char* val, int len, int format
);

template<>
pq_async::timestamp val_from_pgparam<pq_async::timestamp>(
    int oid, char* val, int len, int format
);
template<>
pq_async::timestamp_tz val_from_pgparam<pq_async::timestamp_tz>(
    int oid, char* val, int len, int format
);

template<>
pq_async::date val_from_pgparam<pq_async::date>(
    int oid, char* val, int len, int format
);

template<>
pq_async::interval val_from_pgparam<pq_async::interval>(
    int oid, char* val, int len, int format
);

template<>
pq_async::numeric val_from_pgparam<pq_async::numeric>(
    int oid, char* val, int len, int format
);

template<>
pq_async::money val_from_pgparam<pq_async::money>(
    int oid, char* val, int len, int format
);

template<>
pq_async::json val_from_pgparam<pq_async::json>(
    int oid, char* val, int len, int format
);

template<>
std::vector<int8_t> val_from_pgparam<std::vector<int8_t>>(
    int oid, char* val, int len, int format
);

template<>
pq_async::uuid val_from_pgparam<pq_async::uuid>(
    int oid, char* val, int len, int format
);

template<>
pq_async::oid val_from_pgparam<pq_async::oid>(
    int oid, char* val, int len, int format
);

template<>
pq_async::cidr val_from_pgparam<pq_async::cidr>(
    int oid, char* val, int len, int format
);
template<>
pq_async::inet val_from_pgparam<pq_async::inet>(
    int oid, char* val, int len, int format
);
template<>
pq_async::macaddr val_from_pgparam<pq_async::macaddr>(
    int oid, char* val, int len, int format
);
template<>
pq_async::macaddr8 val_from_pgparam<pq_async::macaddr8>(
    int oid, char* val, int len, int format
);
template<>
pq_async::point val_from_pgparam<pq_async::point>(
    int oid, char* val, int len, int format
);
template<>
pq_async::line val_from_pgparam<pq_async::line>(
    int oid, char* val, int len, int format
);
template<>
pq_async::lseg val_from_pgparam<pq_async::lseg>(
    int oid, char* val, int len, int format
);
template<>
pq_async::box val_from_pgparam<pq_async::box>(
    int oid, char* val, int len, int format
);
template<>
pq_async::path val_from_pgparam<pq_async::path>(
    int oid, char* val, int len, int format
);
template<>
pq_async::polygon val_from_pgparam<pq_async::polygon>(
    int oid, char* val, int len, int format
);
template<>
pq_async::circle val_from_pgparam<pq_async::circle>(
    int oid, char* val, int len, int format
);

template<>
pq_async::int4range val_from_pgparam<pq_async::int4range>(
    int oid, char* val, int len, int format
);
template<>
pq_async::int8range val_from_pgparam<pq_async::int8range>(
    int oid, char* val, int len, int format
);
template<>
pq_async::numrange val_from_pgparam<pq_async::numrange>(
    int oid, char* val, int len, int format
);
template<>
pq_async::tsrange val_from_pgparam<pq_async::tsrange>(
    int oid, char* val, int len, int format
);
template<>
pq_async::tstzrange val_from_pgparam<pq_async::tstzrange>(
    int oid, char* val, int len, int format
);
template<>
pq_async::daterange val_from_pgparam<pq_async::daterange>(
    int oid, char* val, int len, int format
);

// template<>
// uuids::uuid val_from_pgparam<uuids::uuid>(
// 	int oid, char* val, int len, int format
// );


// ================================
// pgval_to_...====================
// ================================
bool pgval_to_bool(char* val, int len, int fmt);
std::string pgval_to_string(char* val, int len, int fmt);
int16_t pgval_to_int16(char* val, int len, int fmt);
int32_t pgval_to_int32(char* val, int len, int fmt);
int64_t pgval_to_int64(char* val, int len, int fmt);
float pgval_to_float(char* val, int len, int fmt);
double pgval_to_double(char* val, int len, int fmt);
pq_async::time pgval_to_time(char* val, int len, int fmt);
pq_async::time_tz pgval_to_time_tz(char* val, int len, int fmt);
pq_async::timestamp pgval_to_timestamp(char* val, int len, int fmt);
pq_async::timestamp_tz pgval_to_timestamp_tz(char* val, int len, int fmt);
pq_async::interval pgval_to_interval(char* val, int len, int fmt);
pq_async::date pgval_to_date(char* val, int len, int fmt);
pq_async::numeric pgval_to_numeric(char* val, int len, int fmt);
pq_async::money pgval_to_money(char* val, int len, int fmt);
std::vector<int8_t> pgval_to_bytea(char* val, int len, int fmt);
pq_async::uuid pgval_to_uuid(char* val, int len, int fmt);
pq_async::oid pgval_to_oid(char* val, int len, int fmt);
pq_async::cidr pgval_to_cidr(char* val, int len, int fmt);
pq_async::inet pgval_to_inet(char* val, int len, int fmt);
pq_async::macaddr pgval_to_macaddr(char* val, int len, int fmt);
pq_async::macaddr8 pgval_to_macaddr8(char* val, int len, int fmt);
pq_async::point pgval_to_point(char* val, int len, int fmt);
pq_async::line pgval_to_line(char* val, int len, int fmt);
pq_async::lseg pgval_to_lseg(char* val, int len, int fmt);
pq_async::box pgval_to_box(char* val, int len, int fmt);
pq_async::path pgval_to_path(char* val, int len, int fmt);
pq_async::polygon pgval_to_polygon(char* val, int len, int fmt);
pq_async::circle pgval_to_circle(char* val, int len, int fmt);


template < typename T >
T pgval_to_range(char* val, int len, int fmt)
{
    std::string errMsg = "Convertion from postgres to: \"";
    errMsg.append(typeid(T).name() );
    errMsg.append("\" is not supported.");
    throw pq_async::exception(errMsg);
}
template <>
pq_async::int4range pgval_to_range<pq_async::int4range>(
    char* val, int len, int fmt
);
template <>
pq_async::int8range pgval_to_range<pq_async::int8range>(
    char* val, int len, int fmt
);
template <>
pq_async::numrange pgval_to_range<pq_async::numrange>(
    char* val, int len, int fmt
);
template <>
pq_async::tsrange pgval_to_range<pq_async::tsrange>(
    char* val, int len, int fmt
);
template <>
pq_async::tstzrange pgval_to_range<pq_async::tstzrange>(
    char* val, int len, int fmt
);
template <>
pq_async::daterange pgval_to_range<pq_async::daterange>(
    char* val, int len, int fmt
);

// ================================
// pgval_to_array...===============
// ================================
void get_array_oid_and_dim(char* val, int len, int fmt, int& oid, int& dim);


template <class T, int32_t dim_count>
boost::multi_array<T, dim_count> pgval_to_array(char* val, int len, int fmt)
{
    if(!fmt)
        throw pq_async::exception(PQ_ASYNC_ERR_NO_TEXT_FMT);

    boost::array<int32_t, dim_count> dimSizes;
    
    // dimension count.
    int32_t svrDimCount = 0;
    pq_async::swap4((int32_t*)val, &svrDimCount, false);
    val += 4;

    if(svrDimCount != dim_count)
        throw pq_async::exception(PQ_ASYNC_ERR_DIM_NO_MATCH);
    
    // skip NULL flag.
    val += 4;

    // verify element OID.
    int32_t eleOid = 0;
    pq_async::swap4((int32_t*)val, &eleOid, false);
    val += 4;
    
    //TODO: verify eleOid vs T.
    
    // fetch dim size.
    int32_t eleCount = 1;
    for(int32_t i = 0; i < svrDimCount; ++i){
        int32_t dimVal = 0;
        pq_async::swap4((int32_t*)val, &dimVal, false);
        
        eleCount *= dimVal;

        dimSizes[i] = dimVal;
        val += 4;

        //TODO: maby check what to do with the dimension lower bound.
        val += 4;
    }
    
    boost::multi_array<T, dim_count> result_array(dimSizes);
    
    std::vector<T> values;

    for(int32_t i = 0; i < eleCount; ++i){
        int32_t valLen = 0;
        pq_async::swap4((int32_t*)val, &valLen, false);
        val += 4;

        T value = val_from_pgparam<T>(eleOid, val, valLen, 1); 
        val += valLen;

        values.push_back(value);
    }
    
    result_array.assign(values.begin(), values.end());

    return result_array;
}


// ======================================
// new_parameter implementations...======
// ======================================

pq_async::parameter* new_null_parameter();

/*
template< typename T >
pq_async::parameter* new_parameter(T& value)
{
    std::string msg("Unsupported type '");
    msg += typeid(value).name();
    msg += "'!";
    throw pq_async::exception(msg.c_str());
}
*/

pq_async::parameter* new_parameter(bool);
pq_async::parameter* new_parameter(const std::string& value);
pq_async::parameter* new_parameter(const char* value);
pq_async::parameter* new_parameter(int16_t value);
pq_async::parameter* new_parameter(int32_t value);
pq_async::parameter* new_parameter(int64_t value);
pq_async::parameter* new_parameter(float value);
pq_async::parameter* new_parameter(double value);
pq_async::parameter* new_parameter(const pq_async::time& value);
pq_async::parameter* new_parameter(const pq_async::time_tz& value);
pq_async::parameter* new_parameter(const pq_async::timestamp& value);
pq_async::parameter* new_parameter(const pq_async::timestamp_tz& value);
pq_async::parameter* new_parameter(const pq_async::date& value);
pq_async::parameter* new_parameter(const pq_async::interval& value);
pq_async::parameter* new_parameter(const pq_async::numeric& value);
pq_async::parameter* new_parameter(const pq_async::money& value);
pq_async::parameter* new_parameter(const pq_async::json& value);
pq_async::parameter* new_parameter(const std::vector<int8_t>& value);
pq_async::parameter* new_parameter(const pq_async::uuid& value);
pq_async::parameter* new_parameter(const pq_async::oid& value);
pq_async::parameter* new_parameter(const pq_async::cidr& value);
pq_async::parameter* new_parameter(const pq_async::inet& value);
pq_async::parameter* new_parameter(const pq_async::macaddr& value);
pq_async::parameter* new_parameter(const pq_async::macaddr8& value);
pq_async::parameter* new_parameter(const pq_async::point& value);
pq_async::parameter* new_parameter(const pq_async::line& value);
pq_async::parameter* new_parameter(const pq_async::lseg& value);
pq_async::parameter* new_parameter(const pq_async::box& value);
pq_async::parameter* new_parameter(const pq_async::path& value);
pq_async::parameter* new_parameter(const pq_async::polygon& value);
pq_async::parameter* new_parameter(const pq_async::circle& value);
pq_async::parameter* new_parameter(const pq_async::int4range& value);
pq_async::parameter* new_parameter(const pq_async::int8range& value);
pq_async::parameter* new_parameter(const pq_async::numrange& value);
pq_async::parameter* new_parameter(const pq_async::tsrange& value);
pq_async::parameter* new_parameter(const pq_async::tstzrange& value);
pq_async::parameter* new_parameter(const pq_async::daterange& value);


//template <class T, int32_t dim_count>
// specialisation

void vec_append(std::vector<char> &vec, char* buf, int len);

template< class T, int32_t dim_count>
pq_async::parameter* new_parameter_gen(
    boost::multi_array<T, dim_count> value,
    int32_t eleOid, int32_t arrEleOid
    )
{
    std::vector<char> membuf2;
    
    // dimension count.
    int32_t dimCount = dim_count;
    int32_t outDimCount = 0;
    pq_async::swap4(&dimCount, &outDimCount, true);
    vec_append(membuf2, (char*)&outDimCount, sizeof(outDimCount));

    // null flag.
    int32_t nullFlag = 0;
    int32_t outNullFlag = 0;
    pq_async::swap4(&nullFlag, &outNullFlag, true);
    vec_append(membuf2, (char*)&outNullFlag, sizeof(outNullFlag));

    int32_t outEleOid = 0;
    pq_async::swap4(&eleOid, &outEleOid, true);
    vec_append(membuf2, (char*)&outEleOid, sizeof(outEleOid));
    
    // dimension sizes.
    for(int32_t i = 0; i < dim_count; ++i){
        // dim size.
        int32_t dimEleCnt = value.shape()[i];
        int32_t outDimEleCnt = 0;
        pq_async::swap4(&dimEleCnt, &outDimEleCnt, true);
        vec_append(membuf2, (char*)&outDimEleCnt, sizeof(outDimEleCnt));
    
        // lbound.
        int32_t lBound = 0;
        int32_t outLBound = 0;
        pq_async::swap4(&lBound, &outLBound, true);
        vec_append(membuf2, (char*)&outLBound, sizeof(outLBound));
    }
    
    T* itVal = value.data();
    
    int32_t num_elements = (int32_t)value.num_elements();
    for(int32_t i = 0; i < num_elements; ++i){
        pq_async::parameter* param = 
            pq_async::new_parameter(*((const T*)itVal));
        
        char* val = (char*)param->get_value();
        int32_t len = param->get_length();
        
        int32_t outLen = 0;
        pq_async::swap4(&len, &outLen, true);
        vec_append(membuf2, (char*)&outLen, sizeof(int32_t));
    
        vec_append(membuf2, val, len);
        
        delete param;
        ++itVal;
    }

    int size = membuf2.size();

    char* buf = new char[size];
    for(int32_t i = 0; i < size; ++i)
        buf[i] = membuf2[i];
    
    return new pq_async::parameter(arrEleOid, buf, size, 1);
}

#define PQ_ASYNC_ARRAY_SPEC(__type, __dim, __name) \
typedef boost::multi_array<__type, __dim> arr_ ## __name ## _ ## __dim ## d; \
pq_async::parameter* new_parameter( \
    const boost::multi_array<__type, __dim> & value \
); \
template<> \
boost::multi_array< __type, __dim > \
val_from_pgparam< boost::multi_array< __type, __dim > >( \
int oid, char* val, int len, int format);


#define PQ_ASYNC_ARRAY_DIM(__dim) \
PQ_ASYNC_ARRAY_SPEC(std::string, __dim, string) \
PQ_ASYNC_ARRAY_SPEC(char*, __dim, char) \
PQ_ASYNC_ARRAY_SPEC(int16_t, __dim, int16) \
PQ_ASYNC_ARRAY_SPEC(int32_t, __dim, int32) \
PQ_ASYNC_ARRAY_SPEC(int64_t, __dim, int64) \
PQ_ASYNC_ARRAY_SPEC(float, __dim, float) \
PQ_ASYNC_ARRAY_SPEC(double, __dim, double) \
PQ_ASYNC_ARRAY_SPEC(pq_async::numeric, __dim, numeric) \
PQ_ASYNC_ARRAY_SPEC(pq_async::money, __dim, money) \
PQ_ASYNC_ARRAY_SPEC(pq_async::time, __dim, time) \
PQ_ASYNC_ARRAY_SPEC(pq_async::time_tz, __dim, time_tz) \
PQ_ASYNC_ARRAY_SPEC(pq_async::timestamp, __dim, timestamp) \
PQ_ASYNC_ARRAY_SPEC(pq_async::timestamp_tz, __dim, timestamp_tz) \
PQ_ASYNC_ARRAY_SPEC(pq_async::date, __dim, date) \
PQ_ASYNC_ARRAY_SPEC(pq_async::interval, __dim, interval) \
PQ_ASYNC_ARRAY_SPEC(pq_async::json, __dim, json) \
PQ_ASYNC_ARRAY_SPEC(std::vector<int8_t>, __dim, bytea) \
PQ_ASYNC_ARRAY_SPEC(pq_async::uuid, __dim, uuid) \
PQ_ASYNC_ARRAY_SPEC(pq_async::oid, __dim, oid) \
PQ_ASYNC_ARRAY_SPEC(pq_async::cidr, __dim, cidr) \
PQ_ASYNC_ARRAY_SPEC(pq_async::inet, __dim, inet) \
PQ_ASYNC_ARRAY_SPEC(pq_async::macaddr, __dim, macaddr) \
PQ_ASYNC_ARRAY_SPEC(pq_async::macaddr8, __dim, macaddr8) \
PQ_ASYNC_ARRAY_SPEC(pq_async::point, __dim, point) \
PQ_ASYNC_ARRAY_SPEC(pq_async::line, __dim, line) \
PQ_ASYNC_ARRAY_SPEC(pq_async::lseg, __dim, lseg) \
PQ_ASYNC_ARRAY_SPEC(pq_async::box, __dim, box) \
PQ_ASYNC_ARRAY_SPEC(pq_async::path, __dim, path) \
PQ_ASYNC_ARRAY_SPEC(pq_async::polygon, __dim, polygon) \
PQ_ASYNC_ARRAY_SPEC(pq_async::circle, __dim, circle) \
PQ_ASYNC_ARRAY_SPEC(pq_async::int4range, __dim, int4range) \
PQ_ASYNC_ARRAY_SPEC(pq_async::int8range, __dim, int8range) \
PQ_ASYNC_ARRAY_SPEC(pq_async::numrange, __dim, numrange) \
PQ_ASYNC_ARRAY_SPEC(pq_async::tsrange, __dim, tsrange) \
PQ_ASYNC_ARRAY_SPEC(pq_async::tstzrange, __dim, tstzrange) \
PQ_ASYNC_ARRAY_SPEC(pq_async::daterange, __dim, daterange) 

PQ_ASYNC_ARRAY_DIM(1);
PQ_ASYNC_ARRAY_DIM(2);
PQ_ASYNC_ARRAY_DIM(3);
PQ_ASYNC_ARRAY_DIM(4);
PQ_ASYNC_ARRAY_DIM(5);



/*!
 * \brief Parameter container
 * 
 */
class parameters
{
public:
    /*!
     * \brief Construct a new parameters object
     * 
     */
    parameters()
        : _p_types(nullptr), _p_values(nullptr),
        _p_lengths(nullptr), _p_formats(nullptr)
    {
    }
    
    /*!
     * \brief Construct a new parameters object
     * 
     * \tparam PARAMS 
     * \param args 
     */
    template<typename... PARAMS >
    parameters(const PARAMS&... args)
        : _p_types(nullptr), _p_values(nullptr),
        _p_lengths(nullptr), _p_formats(nullptr)
    {
        push_back<sizeof...(PARAMS)>(args...);
    }
    
    /*!
     * \brief Copy construct a new parameters object
     * 
     * \param b 
     */
    parameters(const parameters& b)
        : _p_types(nullptr), _p_values(nullptr),
        _p_lengths(nullptr), _p_formats(nullptr)
    {
        copy_from(b);
    }
    /*!
     * \brief Copy assign a parameters object
     * 
     * \param b 
     * \return parameters& 
     */
    parameters& operator=(const parameters& b)
    {
        copy_from(b);
        return *this;
    }
    /*!
     * \brief Move construct a new parameters object
     * 
     * \param b 
     */
    parameters(parameters&& b)
        : _p_types(std::move(b._p_types)),
        _p_values(std::move(b._p_values)),
        _p_lengths(std::move(b._p_lengths)),
        _p_formats(std::move(b._p_formats))
    {
        _p = std::move(b._p);
    }
    /*!
     * \brief Move assign a parameters object
     * 
     * \param b 
     * \return parameters& 
     */
    parameters& operator=(parameters&& b)
    {
        _p = std::move(b._p);
        _p_types = std::move(b._p_types);
        _p_values = std::move(b._p_values);
        _p_lengths = std::move(b._p_lengths);
        _p_formats = std::move(b._p_formats);
        
        return *this;
    }
    
    /*!
     * \brief Destroy the parameters object
     * 
     */
    ~parameters()
    {
        this->cleanup(true);
    }
    
    /*!
     * \brief get the parameters list size
     * 
     * \return int 
     */
    int size() const
    {
        return (int)_p.size();
    }
    
    Oid* types()
    {
        this->init();
        return _p_types;
    }
    const char** values()
    {
        this->init();
        return (const char**)_p_values;
    }
    int* lengths()
    {
        this->init();
        return _p_lengths;
    }
    int* formats()
    {
        this->init();
        return _p_formats;
    }
    
    /*!
     * \brief 
     * 
     * \param p 
     */
    void push_back(parameter* p)
    {
        this->cleanup();
        _p.push_back(p);
    }
    
    /*!
     * \brief the push_back method that sould only be called by the
     * variadic push_back function
     */
    template<int SIZE = 0,
        typename std::enable_if<(SIZE == 0), int32_t>::type = 0
    >
    void push_back() {}
    
    template<typename... PARAMS>
    void push_back(const PARAMS& ...args)
    {
        push_back<sizeof...(PARAMS)>(args...);
    }

    template<int SIZE, typename T, typename... PARAMS,
        typename std::enable_if<(SIZE == 0), int32_t>::type = 0
    >
    void push_back(const T& /*value*/, const PARAMS& .../*args*/){}
    
    /*!
     * \brief creates and push_back parameter object in the parameters object
     * will be called recursively until all provided parameters have been 
     * processed
     * \tparam T Type of current processed value
     * \tparam PARAMS Type of parameters pack
     * \param value current processed value
     * \param args parameters pack
     */
    template<int SIZE, typename T, typename... PARAMS,
        typename std::enable_if<(SIZE > 0), int32_t>::type = 0
    >
    void push_back(const T& value, const PARAMS& ...args)
    {
        this->cleanup();
        _p.emplace_back(pq_async::new_parameter(value));
        push_back<SIZE -1>(args...);
    }
    
    /*!
     * \brief replace a parameter at the specified position
     * 
     * \tparam T value Type
     * \param pos parameter position to replace
     * \param value new parameter value
     */
    template<typename T>
    void replace(size_t pos, const T& value)
    {
        if(pos >= _p.size())
            throw pq_async::exception("Index out of bound");
        
        this->cleanup();
        
        delete _p[pos];
        pq_async::parameter* param = pq_async::new_parameter(value);
        _p[pos] = param;
    }
    
    /*!
     * \brief remove parameter at the specified position
     * 
     * \param pos the position of the parameter to remove
     */
    void remove_at(size_t pos)
    {
        if(pos >= _p.size())
            throw pq_async::exception("Index out of bound");
        
        _p.erase(_p.begin() + pos);
    }
    
    /*!
     * \brief Get the parameter object from the specified position
     * 
     * \param pos 
     * \return const parameter* 
     */
    const parameter* get_parameter(size_t pos)
    {
        if(pos >= _p.size())
            throw pq_async::exception("Index out of bound");
        
        return _p[pos];
    }
    
private:
    void copy_from(const parameters& b)
    {
        for(size_t i = 0; i < b._p.size(); ++i){
            parameter* p = b._p[i];
            char* val = new char[p->get_length()];
            std::copy(
                p->get_value(), p->get_value() +p->get_length(),
                val
            );
            _p.push_back(
                new parameter(
                    p->get_oid(),
                    val,
                    p->get_length(),
                    p->get_format()
                )
            );
        }
    }
    void init()
    {
        if(_p_types)
            return;
        
        int np = (int)_p.size();
        _p_types = new Oid[np];
        _p_values = new const char*[np];
        _p_lengths = new int[np];
        _p_formats = new int[np];
        
        for(size_t i = 0; i < _p.size(); ++i){
            parameter* param = _p[i];
            
            _p_types[i] = param->get_oid();
            _p_values[i] = param->get_value();
            _p_lengths[i] = param->get_length();
            _p_formats[i] = param->get_format();
        }
    }
    
    void cleanup(bool clean_params = false)
    {
        if(_p_types){
            delete[] _p_types;
            delete[] _p_values;
            delete[] _p_lengths;
            delete[] _p_formats;
        }
        
        if(clean_params)
        for(size_t i = 0; i < _p.size(); ++i)
            delete _p[i];
    }
    
    std::vector<parameter*> _p;
    Oid* _p_types;
    const char** _p_values;
    int* _p_lengths;
    int* _p_formats;
};


} //namespace pq_async

//#pragma GCC diagnostic pop

#endif //_libpq_async_parameters_h
