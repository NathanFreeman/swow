/*
  +--------------------------------------------------------------------------+
  | Swow                                                                     |
  +--------------------------------------------------------------------------+
  | Licensed under the Apache License, Version 2.0 (the "License");          |
  | you may not use this file except in compliance with the License.         |
  | You may obtain a copy of the License at                                  |
  | http://www.apache.org/licenses/LICENSE-2.0                               |
  | Unless required by applicable law or agreed to in writing, software      |
  | distributed under the License is distributed on an "AS IS" BASIS,        |
  | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. |
  | See the License for the specific language governing permissions and      |
  | limitations under the License. See accompanying LICENSE file.            |
  +--------------------------------------------------------------------------+
  | Author: Twosee <twosee@php.net>                                          |
  +--------------------------------------------------------------------------+
 */

#include "swow_channel.h"

SWOW_API zend_class_entry *swow_channel_ce;
SWOW_API zend_object_handlers swow_channel_handlers;
SWOW_API zend_class_entry *swow_channel_exception_ce;

SWOW_API zend_class_entry *swow_selector_ce;
SWOW_API zend_object_handlers swow_selector_handlers;
SWOW_API zend_class_entry *swow_selector_exception_ce;

#define SWOW_CHANNEL_GETTER_INTERNAL(object, s_channel, channel) \
    swow_channel_t *s_channel = swow_channel_get_from_object(object); \
    cat_channel_t *channel = &s_channel->channel

#define CHANNEL_HAS_CONSTRUCTED(channel) ((channel)->dtor == (cat_channel_data_dtor_t) zval_ptr_dtor)

static zend_object *swow_channel_create_object(zend_class_entry *ce)
{
    swow_channel_t *s_channel = swow_object_alloc(swow_channel_t, ce, swow_channel_handlers);

    s_channel->channel.dtor = NULL;

    return &s_channel->std;
}

static void swow_channel_dtor_object(zend_object *object)
{
    /* try to call __destruct first */
    zend_objects_destroy_object(object);

    /* force close the channel (as far as possible before free_obj) */
    SWOW_CHANNEL_GETTER_INTERNAL(object, s_channel, channel);

    if (UNEXPECTED(!CHANNEL_HAS_CONSTRUCTED(channel))) {
        return;
    }

    cat_channel_close(channel);
}

static void swow_channel_free_object(zend_object *object)
{
    SWOW_CHANNEL_GETTER_INTERNAL(object, s_channel, channel);

    if (EXPECTED(CHANNEL_HAS_CONSTRUCTED(channel))) {
        cat_channel_close(channel);
    }

    zend_object_std_dtor(&s_channel->std);
}

#define SWOW_CHANNEL_CHECK(channel) do { \
    if (UNEXPECTED(!CHANNEL_HAS_CONSTRUCTED(channel))) { \
        zend_throw_error(NULL, "%s must construct first", ZEND_THIS_NAME); \
        RETURN_THROWS(); \
    } \
} while (0)

#define SWOW_CHANNEL_GETTER(s_channel, channel) \
        SWOW_CHANNEL_GETTER_INTERNAL(Z_OBJ_P(ZEND_THIS), s_channel, channel); \

#define SWOW_CHANNEL_GETTER_CONSTRUCTED(s_channel, channel) \
        SWOW_CHANNEL_GETTER(s_channel, channel); \
        SWOW_CHANNEL_CHECK(channel)

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Swow_Channel___construct, 0, 0, 0)
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, capacity, IS_LONG, 0, "0")
ZEND_END_ARG_INFO()

static PHP_METHOD(Swow_Channel, __construct)
{
    SWOW_CHANNEL_GETTER(s_channel, channel);
    zend_long capacity = 0;

    if (UNEXPECTED(CHANNEL_HAS_CONSTRUCTED(channel))) {
        zend_throw_error(NULL, "%s can be constructed only once", ZEND_THIS_NAME);
        RETURN_THROWS();
    }

    ZEND_PARSE_PARAMETERS_START(0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(capacity)
    ZEND_PARSE_PARAMETERS_END();

    if (capacity == -1) {
        capacity = CAT_CHANNEL_SIZE_MAX;
    }
    if (UNEXPECTED(capacity < 0)) {
        zend_argument_value_error(1, "can not be negative");
        RETURN_THROWS();
    }

    channel = cat_channel_create(channel, capacity, sizeof(zval), (cat_channel_data_dtor_t) zval_ptr_dtor);

    if (UNEXPECTED(channel == NULL)) {
        swow_throw_exception_with_last(swow_channel_exception_ce);
        RETURN_THROWS();
    }
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Swow_Channel_push, 0, 1, IS_STATIC, 0)
    ZEND_ARG_TYPE_INFO(0, data, IS_MIXED, 0)
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, timeout, IS_LONG, 0, "-1")
ZEND_END_ARG_INFO()

static PHP_METHOD(Swow_Channel, push)
{
    SWOW_CHANNEL_GETTER_CONSTRUCTED(s_channel, channel);
    zval *z_data;
    zend_long timeout = -1;
    cat_bool_t ret;

    ZEND_PARSE_PARAMETERS_START(1, 2)
        Z_PARAM_ZVAL(z_data)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(timeout)
    ZEND_PARSE_PARAMETERS_END();

    Z_TRY_ADDREF_P(z_data);
    ret = cat_channel_push(channel, z_data, timeout);
    if (UNEXPECTED(!ret)) {
        Z_TRY_DELREF_P(z_data);
        swow_throw_exception_with_last(swow_channel_exception_ce);
        RETURN_THROWS();
    }

    RETURN_THIS();
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Swow_Channel_pop, 0, 0, IS_MIXED, 0)
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, timeout, IS_LONG, 0, "-1")
ZEND_END_ARG_INFO()

static PHP_METHOD(Swow_Channel, pop)
{
    SWOW_CHANNEL_GETTER_CONSTRUCTED(s_channel, channel);
    zend_long timeout = -1;
    cat_bool_t ret;

    ZEND_PARSE_PARAMETERS_START(0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(timeout)
    ZEND_PARSE_PARAMETERS_END();

    ret = cat_channel_pop(channel, return_value, timeout);

    if (UNEXPECTED(!ret)) {
        swow_throw_exception_with_last(swow_channel_exception_ce);
        RETURN_THROWS();
    }
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Swow_Channel_close, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

static PHP_METHOD(Swow_Channel, close)
{
    SWOW_CHANNEL_GETTER_CONSTRUCTED(s_channel, channel);

    ZEND_PARSE_PARAMETERS_NONE();

    cat_channel_close(channel);
}

/* status */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Swow_Channel_getCapacity, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

static PHP_METHOD(Swow_Channel, getCapacity)
{
    SWOW_CHANNEL_GETTER_CONSTRUCTED(s_channel, channel);

    ZEND_PARSE_PARAMETERS_NONE();

    RETURN_LONG(channel->capacity);
}

#define arginfo_class_Swow_Channel_getLength arginfo_class_Swow_Channel_getCapacity

static PHP_METHOD(Swow_Channel, getLength)
{
    SWOW_CHANNEL_GETTER_CONSTRUCTED(s_channel, channel);

    ZEND_PARSE_PARAMETERS_NONE();

    RETURN_LONG(channel->length);
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Swow_Channel_isAvailable, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

static PHP_METHOD(Swow_Channel, isAvailable)
{
    SWOW_CHANNEL_GETTER_CONSTRUCTED(s_channel, channel);

    ZEND_PARSE_PARAMETERS_NONE();

    RETURN_BOOL(cat_channel_is_available(channel));
}

#define arginfo_class_Swow_Channel_hasProducers arginfo_class_Swow_Channel_isAvailable

static PHP_METHOD(Swow_Channel, hasProducers)
{
    SWOW_CHANNEL_GETTER_CONSTRUCTED(s_channel, channel);

    ZEND_PARSE_PARAMETERS_NONE();

    RETURN_BOOL(cat_channel_has_producers(channel));
}

#define arginfo_class_Swow_Channel_hasConsumers arginfo_class_Swow_Channel_isAvailable

static PHP_METHOD(Swow_Channel, hasConsumers)
{
    SWOW_CHANNEL_GETTER_CONSTRUCTED(s_channel, channel);

    ZEND_PARSE_PARAMETERS_NONE();

    RETURN_BOOL(cat_channel_has_consumers(channel));
}

#define arginfo_class_Swow_Channel_isEmpty arginfo_class_Swow_Channel_isAvailable

static PHP_METHOD(Swow_Channel, isEmpty)
{
    SWOW_CHANNEL_GETTER_CONSTRUCTED(s_channel, channel);

    ZEND_PARSE_PARAMETERS_NONE();

    RETURN_BOOL(cat_channel_is_empty(channel));
}

#define arginfo_class_Swow_Channel_isFull arginfo_class_Swow_Channel_isAvailable

static PHP_METHOD(Swow_Channel, isFull)
{
    SWOW_CHANNEL_GETTER_CONSTRUCTED(s_channel, channel);

    ZEND_PARSE_PARAMETERS_NONE();

    RETURN_BOOL(cat_channel_is_full(channel));
}

#define arginfo_class_Swow_Channel_isReadable arginfo_class_Swow_Channel_isAvailable

static PHP_METHOD(Swow_Channel, isReadable)
{
    SWOW_CHANNEL_GETTER_CONSTRUCTED(s_channel, channel);

    ZEND_PARSE_PARAMETERS_NONE();

    RETURN_BOOL(cat_channel_is_readable(channel));
}

#define arginfo_class_Swow_Channel_isWritable arginfo_class_Swow_Channel_isAvailable

static PHP_METHOD(Swow_Channel, isWritable)
{
    SWOW_CHANNEL_GETTER_CONSTRUCTED(s_channel, channel);

    ZEND_PARSE_PARAMETERS_NONE();

    RETURN_BOOL(cat_channel_is_writable(channel));
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Swow_Channel___debugInfo, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

static PHP_METHOD(Swow_Channel, __debugInfo)
{
    SWOW_CHANNEL_GETTER(s_channel, channel);
    zval z_debug_info;

    ZEND_PARSE_PARAMETERS_NONE();

    if (UNEXPECTED(!CHANNEL_HAS_CONSTRUCTED(channel))) {
        return;
    }

    array_init(&z_debug_info);
    add_assoc_long(&z_debug_info, "capacity", channel->capacity);
    add_assoc_long(&z_debug_info, "length", channel->length);
    add_assoc_bool(&z_debug_info, "readable", cat_channel_is_readable(channel));
    add_assoc_bool(&z_debug_info, "writable", cat_channel_is_writable(channel));

    RETURN_DEBUG_INFO_WITH_PROPERTIES(&z_debug_info);
}

static const zend_function_entry swow_channel_methods[] = {
    PHP_ME(Swow_Channel, __construct,  arginfo_class_Swow_Channel___construct,  ZEND_ACC_PUBLIC)
    PHP_ME(Swow_Channel, push,         arginfo_class_Swow_Channel_push,         ZEND_ACC_PUBLIC)
    PHP_ME(Swow_Channel, pop,          arginfo_class_Swow_Channel_pop,          ZEND_ACC_PUBLIC)
    PHP_ME(Swow_Channel, close,        arginfo_class_Swow_Channel_close,        ZEND_ACC_PUBLIC)
    /* status */
    PHP_ME(Swow_Channel, getCapacity,  arginfo_class_Swow_Channel_getCapacity,  ZEND_ACC_PUBLIC)
    PHP_ME(Swow_Channel, getLength,    arginfo_class_Swow_Channel_getLength,    ZEND_ACC_PUBLIC)
    PHP_ME(Swow_Channel, isAvailable,  arginfo_class_Swow_Channel_isAvailable,  ZEND_ACC_PUBLIC)
    PHP_ME(Swow_Channel, hasProducers, arginfo_class_Swow_Channel_hasProducers, ZEND_ACC_PUBLIC)
    PHP_ME(Swow_Channel, hasConsumers, arginfo_class_Swow_Channel_hasConsumers, ZEND_ACC_PUBLIC)
    PHP_ME(Swow_Channel, isEmpty,      arginfo_class_Swow_Channel_isEmpty,      ZEND_ACC_PUBLIC)
    PHP_ME(Swow_Channel, isFull,       arginfo_class_Swow_Channel_isFull,       ZEND_ACC_PUBLIC)
    PHP_ME(Swow_Channel, isReadable,   arginfo_class_Swow_Channel_isReadable,   ZEND_ACC_PUBLIC)
    PHP_ME(Swow_Channel, isWritable,   arginfo_class_Swow_Channel_isWritable,   ZEND_ACC_PUBLIC)
    /* magic */
    PHP_ME(Swow_Channel, __debugInfo,  arginfo_class_Swow_Channel___debugInfo,  ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static HashTable *swow_channel_get_gc(zend_object *object, zval **gc_data, int *gc_count)
{
    SWOW_CHANNEL_GETTER_INTERNAL(object, s_channel, channel);

    if (channel->length == 0) {
        *gc_data = NULL;
        *gc_count = 0;
        return zend_std_get_properties(object);
    }

    zend_get_gc_buffer *zgc_buffer = zend_get_gc_buffer_create();

    CAT_QUEUE_FOREACH_DATA_START(&channel->u.buffered.storage, cat_channel_bucket_t, node, bucket) {
        zend_get_gc_buffer_add_zval(zgc_buffer, (zval *) bucket->data);
    } CAT_QUEUE_FOREACH_DATA_END();

    zend_get_gc_buffer_use(zgc_buffer, gc_data, gc_count);

    return zend_std_get_properties(object);
}

/* select */

static void swow_selector_release_requests(swow_selector_t *selector)
{
    cat_channel_select_request_t *request;
    uint32_t n;

    for (n = 0, request = selector->requests; n < selector->count; n++, request++) {
        zend_object_release(&(swow_channel_get_from_handle(request->channel)->std));
        if (request->opcode == CAT_CHANNEL_SELECT_EVENT_PUSH) {
            zval_ptr_dtor((zval *) request->data.common);
        }
    }

    selector->count = 0;
}

static void swow_selector_release_response(swow_selector_t *selector)
{
    zval_ptr_dtor(&selector->z_data);
}

static zend_object *swow_selector_create_object(zend_class_entry *ce)
{
    swow_selector_t *selector = swow_object_alloc(swow_selector_t, ce, swow_selector_handlers);

    selector->count = 0;
    selector->requests = selector->_requests;
    selector->size = CAT_ARRAY_SIZE(selector->_requests);
    selector->last_event = CAT_CHANNEL_SELECT_EVENT_PUSH;
    ZVAL_NULL(&selector->z_data);

    return &selector->std;
}

static void swow_selector_free_object(zend_object *object)
{
    swow_selector_t *selector = swow_selector_get_from_object(object);

    swow_selector_release_requests(selector);
    if (selector->requests != selector->_requests) {
        efree(selector->requests);
    }
    swow_selector_release_response(selector);

    zend_object_std_dtor(&selector->std);
}

#define getThisSelector() swow_selector_get_from_object(Z_OBJ_P(ZEND_THIS))

static PHP_METHOD_EX(Swow_Selector, add, zend_object *channel_object, zval *z_data)
{
    swow_selector_t *selector = getThisSelector();
    swow_channel_t *s_channel = swow_channel_get_from_object(channel_object);
    cat_channel_t *channel = &s_channel->channel;
    cat_channel_select_request_t *requests, *request;
    zval *z_storage, *z_bucket;

    SWOW_CHANNEL_CHECK(channel);

    requests = selector->requests;
    if (requests == selector->_requests) {
        z_storage = selector->z_storage;
    } else {
        z_storage = (zval *) (selector->requests + selector->size);
    }

    if (UNEXPECTED(selector->count == selector->size)) {
        size_t n;
        /* extend size to 2x */
        selector->size += selector->size;
        selector->requests = emalloc((sizeof(*request) + sizeof(*z_bucket)) * selector->size);
        memcpy(selector->requests,                  requests, sizeof(*request) * selector->count);
        memcpy(selector->requests + selector->size, z_storage, sizeof(*z_bucket) * selector->count);
        if (requests != selector->_requests) {
            efree(requests);
        }
        requests = selector->requests;
        z_storage = (zval *) (requests + selector->size);
        for (n = 0, request = requests, z_bucket = z_storage; n < selector->count; n++, request++, z_bucket++) {
            request->data.common = z_bucket;
        }
    }

    request = &requests[selector->count];
    z_bucket = &z_storage[selector->count];
    /* copy channel */
    GC_ADDREF(&s_channel->std);
    request->channel = channel;
    /* solve data */
    if (z_data != NULL) {
        request->opcode = CAT_CHANNEL_SELECT_EVENT_PUSH;
        ZVAL_COPY(z_bucket, z_data);
        request->data.in = z_bucket;
    } else {
        request->opcode = CAT_CHANNEL_SELECT_EVENT_POP;
        ZVAL_UNDEF(z_bucket);
        request->data.out = z_bucket;
    }
    /* count++ */
    selector->count++;

    RETURN_THIS();
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Swow_Selector_push, 0, 2, IS_STATIC, 0)
    ZEND_ARG_OBJ_INFO(0, channel, Swow\\Channel, 0)
    ZEND_ARG_TYPE_INFO(0, data, IS_MIXED, 0)
ZEND_END_ARG_INFO()

static PHP_METHOD(Swow_Selector, push)
{
    zend_object *channel_object;
    zval *z_data;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_OBJ_OF_CLASS(channel_object, swow_channel_ce)
        Z_PARAM_ZVAL(z_data)
    ZEND_PARSE_PARAMETERS_END();

    PHP_METHOD_CALL(Swow_Selector, add, channel_object, z_data);
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Swow_Selector_pop, 0, 1, IS_STATIC, 0)
    ZEND_ARG_OBJ_INFO(0, channel, Swow\\Channel, 0)
ZEND_END_ARG_INFO()

static PHP_METHOD(Swow_Selector, pop)
{
    zend_object *channel_object;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJ_OF_CLASS(channel_object, swow_channel_ce)
    ZEND_PARSE_PARAMETERS_END();

    PHP_METHOD_CALL(Swow_Selector, add, channel_object, NULL);
}

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_class_Swow_Selector_commit, 0, 0, Swow\\Channel, 0)
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, timeout, IS_LONG, 0, "-1")
ZEND_END_ARG_INFO()

static PHP_METHOD(Swow_Selector, commit)
{
    swow_selector_t *selector = getThisSelector();
    zend_long timeout = -1;
    cat_channel_select_response_t *response;

    ZEND_PARSE_PARAMETERS_START(0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(timeout)
    ZEND_PARSE_PARAMETERS_END();

    response = cat_channel_select(selector->requests, selector->count, timeout);

    /* release the last response */
    swow_selector_release_response(selector);

    /* update the response info */
    if (EXPECTED(response != NULL)) {
        zend_object *channel = &(swow_channel_get_from_handle(response->channel)->std);
        zval *z_data = (zval *) response->data.common;
        RETVAL_OBJ_COPY(channel);
        selector->last_event = response->opcode;
        if (response->opcode == CAT_CHANNEL_SELECT_EVENT_PUSH) {
            ZVAL_UNDEF(&selector->z_data);
        } else /* if (response->opcode == CAT_CHANNEL_SELECT_EVENT_POP) */ {
            ZVAL_COPY_VALUE(&selector->z_data, z_data);
        }
        /* data has been consumed or copied */
        ZVAL_UNDEF(z_data);
    } else {
        ZVAL_UNDEF(&selector->z_data);
    }

    /* reset */
    swow_selector_release_requests(selector);

    /* handle error */
    if (UNEXPECTED(response == NULL || response->error)) {
        swow_throw_call_exception_with_last(swow_selector_exception_ce);
        RETURN_THROWS();
    }
}

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Swow_Selector_fetch, 0, 0, IS_MIXED, 0)
ZEND_END_ARG_INFO()

static PHP_METHOD(Swow_Selector, fetch)
{
    swow_selector_t *selector = getThisSelector();
    zval *z_data = &selector->z_data;

    ZEND_PARSE_PARAMETERS_NONE();

    if (UNEXPECTED(Z_TYPE_P(z_data) == IS_UNDEF)) {
        zend_throw_error(NULL, "No data");
        RETURN_THROWS();
    }

    RETVAL_ZVAL(z_data, 0, 0);

    ZVAL_UNDEF(z_data);
}

#define arginfo_class_Swow_Selector_getLastEvent arginfo_class_Swow_Channel_getCapacity

static PHP_METHOD(Swow_Selector, getLastEvent)
{
    swow_selector_t *selector = getThisSelector();

    ZEND_PARSE_PARAMETERS_NONE();

    RETURN_LONG(selector->last_event);
}

static const zend_function_entry swow_selector_methods[] = {
    PHP_ME(Swow_Selector, push,         arginfo_class_Swow_Selector_push,         ZEND_ACC_PUBLIC)
    PHP_ME(Swow_Selector, pop,          arginfo_class_Swow_Selector_pop,          ZEND_ACC_PUBLIC)
    PHP_ME(Swow_Selector, commit,       arginfo_class_Swow_Selector_commit,       ZEND_ACC_PUBLIC)
    PHP_ME(Swow_Selector, fetch,        arginfo_class_Swow_Selector_fetch,        ZEND_ACC_PUBLIC)
    PHP_ME(Swow_Selector, getLastEvent, arginfo_class_Swow_Selector_getLastEvent, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static HashTable *swow_selector_get_gc(zend_object *object, zval **gc_data, int *gc_count)
{
    swow_selector_t *selector = swow_selector_get_from_object(object);
    cat_channel_select_request_t *request;
    uint32_t n;

    if (selector->count == 0 && !Z_REFCOUNTED_P(&selector->z_data)) {
        *gc_data = NULL;
        *gc_count = 0;
        return zend_std_get_properties(object);
    }

    zend_get_gc_buffer *zgc_buffer = zend_get_gc_buffer_create();

    /* request */
    for (n = 0, request = selector->requests; n < selector->count; n++, request++) {
        if (request->opcode == CAT_CHANNEL_SELECT_EVENT_PUSH) {
            zend_get_gc_buffer_add_zval(zgc_buffer, (zval *) request->data.common);
        }
    }
    /* response */
    zend_get_gc_buffer_add_zval(zgc_buffer, &selector->z_data);

    zend_get_gc_buffer_use(zgc_buffer, gc_data, gc_count);

    return zend_std_get_properties(object);
}

zend_result swow_channel_module_init(INIT_FUNC_ARGS)
{
    /* channel */

    swow_channel_ce = swow_register_internal_class(
        "Swow\\Channel", NULL, swow_channel_methods,
        &swow_channel_handlers, NULL,
        cat_false, cat_false,
        swow_channel_create_object,
        swow_channel_free_object,
        XtOffsetOf(swow_channel_t, std)
    );
    swow_channel_handlers.get_gc = swow_channel_get_gc;
    swow_channel_handlers.dtor_obj = swow_channel_dtor_object;

    swow_channel_exception_ce = swow_register_internal_class(
        "Swow\\ChannelException", swow_exception_ce, NULL, NULL, NULL, cat_true, cat_true, NULL, NULL, 0
    );

    /* selector */

    swow_selector_ce = swow_register_internal_class(
        "Swow\\Selector", NULL, swow_selector_methods,
        &swow_selector_handlers, NULL,
        cat_false, cat_false,
        swow_selector_create_object,
        swow_selector_free_object,
        XtOffsetOf(swow_selector_t, std)
    );
    swow_selector_handlers.get_gc = swow_selector_get_gc;

    zend_declare_class_constant_long(swow_selector_ce, ZEND_STRL("EVENT_PUSH"), CAT_CHANNEL_SELECT_EVENT_PUSH);
    zend_declare_class_constant_long(swow_selector_ce, ZEND_STRL("EVENT_POP"), CAT_CHANNEL_SELECT_EVENT_POP);

    swow_selector_exception_ce = swow_register_internal_class(
        "Swow\\SelectorException", swow_call_exception_ce, NULL, NULL, NULL, cat_true, cat_true, NULL, NULL, 0
    );

    return SUCCESS;
}
