/**
 * -----------------------------------------------------------------------------
 * Project: Fossil Logic
 *
 * This file is part of the Fossil Logic project, which aims to develop
 * high-performance, cross-platform applications and libraries. The code
 * contained herein is licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * Author: Michael Gene Brockus (Dreamer)
 * Date: 04/05/2014
 *
 * Copyright (C) 2014-2025 Fossil Logic. All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include <fossil/pizza/framework.h>
#include "fossil/media/framework.h"


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilities
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_SUITE(c_fson_fixture);

FOSSIL_SETUP(c_fson_fixture) {
    // Setup the test fixture
}

FOSSIL_TEARDOWN(c_fson_fixture) {
    // Teardown the test fixture
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

// Test NULL input handling
FOSSIL_TEST_CASE(c_test_fson_parse_null_input) {
    fossil_media_fson_error_t err = {0};
    fossil_media_fson_value_t *val = fossil_media_fson_parse(NULL, &err);
    ASSUME_ITS_CNULL(val);
    ASSUME_ITS_EQUAL_I32(err.code, FOSSIL_MEDIA_FSON_ERR_INVALID_ARG);
}

// Test whitespace-only input
FOSSIL_TEST_CASE(c_test_fson_parse_whitespace_only) {
    fossil_media_fson_error_t err = {0};
    fossil_media_fson_value_t *val = fossil_media_fson_parse("   \n\t  ", &err);
    ASSUME_ITS_CNULL(val);
    ASSUME_ITS_EQUAL_I32(err.code, FOSSIL_MEDIA_FSON_ERR_PARSE);
}

// Test { null: null } special case
FOSSIL_TEST_CASE(c_test_fson_parse_null_object_special) {
    fossil_media_fson_error_t err = {0};
    const char *json = "{ null: null }";
    fossil_media_fson_value_t *val = fossil_media_fson_parse(json, &err);
    ASSUME_NOT_CNULL(val);
    ASSUME_ITS_EQUAL_CSTR(fossil_media_fson_type_name(val->type), "null");
    fossil_media_fson_free(val);
}

// Test missing key error
FOSSIL_TEST_CASE(c_test_fson_parse_missing_key) {
    fossil_media_fson_error_t err = {0};
    const char *json = "{ : i32: 1 }";
    fossil_media_fson_value_t *val = fossil_media_fson_parse(json, &err);
    ASSUME_ITS_CNULL(val);
    ASSUME_ITS_EQUAL_I32(err.code, FOSSIL_MEDIA_FSON_ERR_PARSE);
}

// Test missing colon after key
FOSSIL_TEST_CASE(c_test_fson_parse_missing_colon_after_key) {
    fossil_media_fson_error_t err = {0};
    const char *json = "{ key i32: 1 }";
    fossil_media_fson_value_t *val = fossil_media_fson_parse(json, &err);
    ASSUME_ITS_CNULL(val);
    ASSUME_ITS_EQUAL_I32(err.code, FOSSIL_MEDIA_FSON_ERR_PARSE);
}

// Test empty object error
FOSSIL_TEST_CASE(c_test_fson_parse_empty_object) {
    fossil_media_fson_error_t err = {0};
    const char *json = "{ }";
    fossil_media_fson_value_t *val = fossil_media_fson_parse(json, &err);
    ASSUME_ITS_CNULL(val);
    ASSUME_ITS_EQUAL_I32(err.code, FOSSIL_MEDIA_FSON_ERR_PARSE);
}

// Test fallback to simple null
FOSSIL_TEST_CASE(c_test_fson_parse_simple_null) {
    fossil_media_fson_error_t err = {0};
    fossil_media_fson_value_t *val = fossil_media_fson_parse("null", &err);
    ASSUME_NOT_CNULL(val);
    ASSUME_ITS_EQUAL_CSTR(fossil_media_fson_type_name(val->type), "null");
    fossil_media_fson_free(val);
}

// Test fallback to simple true/false
FOSSIL_TEST_CASE(c_test_fson_parse_simple_true_false) {
    fossil_media_fson_error_t err = {0};
    fossil_media_fson_value_t *val_true = fossil_media_fson_parse("true", &err);
    fossil_media_fson_value_t *val_false = fossil_media_fson_parse("false", &err);
    ASSUME_NOT_CNULL(val_true);
    ASSUME_NOT_CNULL(val_false);
    ASSUME_ITS_EQUAL_CSTR(fossil_media_fson_type_name(val_true->type), "bool");
    ASSUME_ITS_EQUAL_CSTR(fossil_media_fson_type_name(val_false->type), "bool");
    fossil_media_fson_free(val_true);
    fossil_media_fson_free(val_false);
}

// Test fallback to simple string
FOSSIL_TEST_CASE(c_test_fson_parse_simple_string) {
    fossil_media_fson_error_t err = {0};
    fossil_media_fson_value_t *val = fossil_media_fson_parse("\"abc\"", &err);
    ASSUME_NOT_CNULL(val);
    ASSUME_ITS_EQUAL_CSTR(fossil_media_fson_type_name(val->type), "cstr");
    fossil_media_fson_free(val);
}

// Test fallback to simple integer/float
FOSSIL_TEST_CASE(c_test_fson_parse_simple_number) {
    fossil_media_fson_error_t err = {0};
    fossil_media_fson_value_t *val_int = fossil_media_fson_parse("123", &err);
    fossil_media_fson_value_t *val_float = fossil_media_fson_parse("3.14", &err);
    ASSUME_NOT_CNULL(val_int);
    ASSUME_NOT_CNULL(val_float);
    ASSUME_ITS_EQUAL_CSTR(fossil_media_fson_type_name(val_int->type), "i64");
    ASSUME_ITS_EQUAL_CSTR(fossil_media_fson_type_name(val_float->type), "f64");
    fossil_media_fson_free(val_int);
    fossil_media_fson_free(val_float);
}

// Test unterminated string error
FOSSIL_TEST_CASE(c_test_fson_parse_unterminated_string) {
    fossil_media_fson_error_t err = {0};
    fossil_media_fson_value_t *val = fossil_media_fson_parse("\"abc", &err);
    ASSUME_ITS_CNULL(val);
    ASSUME_ITS_EQUAL_I32(err.code, FOSSIL_MEDIA_FSON_ERR_PARSE);
}

// Test unrecognized value error
FOSSIL_TEST_CASE(c_test_fson_parse_unrecognized_value) {
    fossil_media_fson_error_t err = {0};
    fossil_media_fson_value_t *val = fossil_media_fson_parse("???", &err);
    ASSUME_ITS_CNULL(val);
    ASSUME_ITS_EQUAL_I32(err.code, FOSSIL_MEDIA_FSON_ERR_PARSE);
}

// Edge case: object with quoted key and string value
FOSSIL_TEST_CASE(c_test_fson_parse_object_quoted_key_string_value) {
    fossil_media_fson_error_t err = {0};
    const char *json = "{ \"foo\": cstr: \"bar\" }";
    fossil_media_fson_value_t *val = fossil_media_fson_parse(json, &err);
    ASSUME_NOT_CNULL(val);
    ASSUME_ITS_EQUAL_CSTR(fossil_media_fson_type_name(val->type), "cstr");
    char *out = NULL;
    fossil_media_fson_get_cstr(val, &out);
    ASSUME_ITS_EQUAL_CSTR(out, "bar");
    fossil_media_fson_free(val);
}

// Edge case: object with nested object
FOSSIL_TEST_CASE(c_test_fson_parse_nested_object) {
    fossil_media_fson_error_t err = {0};
    const char *json = "{ user: object: { id: i32: 42 } }";
    fossil_media_fson_value_t *val = fossil_media_fson_parse(json, &err);
    ASSUME_NOT_CNULL(val);
    ASSUME_ITS_EQUAL_CSTR(fossil_media_fson_type_name(val->type), "object");
    fossil_media_fson_value_t *id_val = fossil_media_fson_object_get(val, "id");
    ASSUME_NOT_CNULL(id_val);
    int32_t id = 0;
    fossil_media_fson_get_i32(id_val, &id);
    ASSUME_ITS_EQUAL_I32(id, 42);
    fossil_media_fson_free(val);
}

// Edge case: array of mixed types using array<mix>
FOSSIL_TEST_CASE(c_test_fson_parse_array_mixed_types) {
    fossil_media_fson_error_t err = {0};
    const char *json = "{ mixed: array<mix>: [i32: 1, cstr: \"two\", bool: true] }";
    fossil_media_fson_value_t *val = fossil_media_fson_parse(json, &err);
    ASSUME_NOT_CNULL(val);
    fossil_media_fson_value_t *arr = fossil_media_fson_object_get(val, "mixed");
    ASSUME_NOT_CNULL(arr);
    ASSUME_ITS_EQUAL_I32((int)fossil_media_fson_array_size(arr), 3);
    fossil_media_fson_free(val);
}

// Edge case: array with explicit element type (array<i32>)
FOSSIL_TEST_CASE(c_test_fson_parse_array_explicit_type) {
    fossil_media_fson_error_t err = {0};
    const char *json = "{ numbers: array<i32>: [i32: 1, i32: 2, i32: 3] }";
    fossil_media_fson_value_t *val = fossil_media_fson_parse(json, &err);
    ASSUME_NOT_CNULL(val);
    fossil_media_fson_value_t *arr = fossil_media_fson_object_get(val, "numbers");
    ASSUME_NOT_CNULL(arr);
    int32_t n = 0;
    fossil_media_fson_get_i32(fossil_media_fson_array_get(arr, 1), &n);
    ASSUME_ITS_EQUAL_I32(n, 2);
    fossil_media_fson_free(val);
}

// Edge case: array with empty brackets
FOSSIL_TEST_CASE(c_test_fson_parse_empty_array) {
    fossil_media_fson_error_t err = {0};
    const char *json = "{ empty: array<i32>: [] }";
    fossil_media_fson_value_t *val = fossil_media_fson_parse(json, &err);
    ASSUME_NOT_CNULL(val);
    fossil_media_fson_value_t *arr = fossil_media_fson_object_get(val, "empty");
    ASSUME_NOT_CNULL(arr);
    ASSUME_ITS_EQUAL_I32((int)fossil_media_fson_array_size(arr), 0);
    fossil_media_fson_free(val);
}

// Edge case: malformed array type (missing >)
FOSSIL_TEST_CASE(c_test_fson_parse_malformed_array_type) {
    fossil_media_fson_error_t err = {0};
    const char *json = "{ bad: array<i32: [i32: 1] }";
    fossil_media_fson_value_t *val = fossil_media_fson_parse(json, &err);
    ASSUME_ITS_CNULL(val);
    ASSUME_ITS_EQUAL_I32(err.code, FOSSIL_MEDIA_FSON_ERR_TYPE);
}

// Edge case: object with unknown type
FOSSIL_TEST_CASE(c_test_fson_parse_unknown_type) {
    fossil_media_fson_error_t err = {0};
    const char *json = "{ foo: unknown: 123 }";
    fossil_media_fson_value_t *val = fossil_media_fson_parse(json, &err);
    ASSUME_ITS_CNULL(val);
    ASSUME_ITS_EQUAL_I32(err.code, FOSSIL_MEDIA_FSON_ERR_TYPE);
}

// Edge case: object with missing value
FOSSIL_TEST_CASE(c_test_fson_parse_missing_value) {
    fossil_media_fson_error_t err = {0};
    const char *json = "{ foo: i32: }";
    fossil_media_fson_value_t *val = fossil_media_fson_parse(json, &err);
    ASSUME_ITS_CNULL(val);
    ASSUME_ITS_EQUAL_I32(err.code, FOSSIL_MEDIA_FSON_ERR_PARSE);
}

// Edge case: object with extra comma
FOSSIL_TEST_CASE(c_test_fson_parse_object_extra_comma) {
    fossil_media_fson_error_t err = {0};
    const char *json = "{ foo: i32: 1, }";
    fossil_media_fson_value_t *val = fossil_media_fson_parse(json, &err);
    // Acceptable in FSON: trailing comma is ignored, so should parse
    ASSUME_NOT_CNULL(val);
    fossil_media_fson_free(val);
}

// Edge case: array with extra comma
FOSSIL_TEST_CASE(c_test_fson_parse_array_extra_comma) {
    fossil_media_fson_error_t err = {0};
    const char *json = "[i32: 1, i32: 2, ]";
    fossil_media_fson_value_t *val = fossil_media_fson_parse(json, &err);
    // Acceptable in FSON: trailing comma is ignored, so should parse
    ASSUME_NOT_CNULL(val);
    fossil_media_fson_free(val);
}

// Edge case: parse hex, octal, binary numbers
FOSSIL_TEST_CASE(c_test_fson_parse_hex_oct_bin) {
    fossil_media_fson_error_t err = {0};
    const char *json = "{ hex: hex: 0xFF, oct: oct: 0o755, bin: bin: 0b1010 }";
    fossil_media_fson_value_t *val = fossil_media_fson_parse(json, &err);
    ASSUME_NOT_CNULL(val);
    fossil_media_fson_value_t *hex_val = fossil_media_fson_object_get(val, "hex");
    fossil_media_fson_value_t *oct_val = fossil_media_fson_object_get(val, "oct");
    fossil_media_fson_value_t *bin_val = fossil_media_fson_object_get(val, "bin");
    uint64_t u = 0;
    fossil_media_fson_get_hex(hex_val, &u);
    ASSUME_ITS_EQUAL_I32((int)u, 255);
    fossil_media_fson_get_oct(oct_val, &u);
    ASSUME_ITS_EQUAL_I32((int)u, 493);
    fossil_media_fson_get_bin(bin_val, &u);
    ASSUME_ITS_EQUAL_I32((int)u, 10);
    fossil_media_fson_free(val);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *
FOSSIL_TEST_GROUP(c_fson_tests) {
    FOSSIL_TEST_ADD(c_fson_fixture, c_test_fson_parse_null_input);
    FOSSIL_TEST_ADD(c_fson_fixture, c_test_fson_parse_whitespace_only);
    FOSSIL_TEST_ADD(c_fson_fixture, c_test_fson_parse_null_object_special);
    FOSSIL_TEST_ADD(c_fson_fixture, c_test_fson_parse_missing_key);
    FOSSIL_TEST_ADD(c_fson_fixture, c_test_fson_parse_missing_colon_after_key);
    FOSSIL_TEST_ADD(c_fson_fixture, c_test_fson_parse_empty_object);
    FOSSIL_TEST_ADD(c_fson_fixture, c_test_fson_parse_simple_null);
    FOSSIL_TEST_ADD(c_fson_fixture, c_test_fson_parse_simple_true_false);
    FOSSIL_TEST_ADD(c_fson_fixture, c_test_fson_parse_simple_string);
    FOSSIL_TEST_ADD(c_fson_fixture, c_test_fson_parse_simple_number);
    FOSSIL_TEST_ADD(c_fson_fixture, c_test_fson_parse_unterminated_string);
    FOSSIL_TEST_ADD(c_fson_fixture, c_test_fson_parse_unrecognized_value);

    FOSSIL_TEST_ADD(c_fson_fixture, c_test_fson_parse_object_quoted_key_string_value);
    FOSSIL_TEST_ADD(c_fson_fixture, c_test_fson_parse_nested_object);
    FOSSIL_TEST_ADD(c_fson_fixture, c_test_fson_parse_array_mixed_types);
    FOSSIL_TEST_ADD(c_fson_fixture, c_test_fson_parse_array_explicit_type);
    FOSSIL_TEST_ADD(c_fson_fixture, c_test_fson_parse_empty_array);
    FOSSIL_TEST_ADD(c_fson_fixture, c_test_fson_parse_malformed_array_type);
    FOSSIL_TEST_ADD(c_fson_fixture, c_test_fson_parse_unknown_type);
    FOSSIL_TEST_ADD(c_fson_fixture, c_test_fson_parse_missing_value);
    FOSSIL_TEST_ADD(c_fson_fixture, c_test_fson_parse_object_extra_comma);
    FOSSIL_TEST_ADD(c_fson_fixture, c_test_fson_parse_array_extra_comma);
    FOSSIL_TEST_ADD(c_fson_fixture, c_test_fson_parse_hex_oct_bin);

    FOSSIL_TEST_REGISTER(c_fson_fixture);
} // end of tests
