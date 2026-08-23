/**
 * ValidatorTest.cpp
 *
 * Tests Validator::isValidEmail, isValidPassword, isValidUsername against
 * the actual implementation in Validator.cpp.
 *
 * Email regex:  ^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$
 * Password:     length >= VALIDATOR_PASSWORD_MIN_LENGTH (6)
 * Username:     !username.empty()
 */

#include <gtest/gtest.h>
#include "../../utils/Validator.h"
#include "../../constants/Strings.h"
#include <string>

// Email tests
TEST(ValidatorTest, Email_Standard_Valid) {
    EXPECT_TRUE(Validator::isValidEmail("user@example.com"));
}
TEST(ValidatorTest, Email_PlusTag_Valid) {
    EXPECT_TRUE(Validator::isValidEmail("user+tag@example.org"));
}
TEST(ValidatorTest, Email_DotInLocalPart_Valid) {
    EXPECT_TRUE(Validator::isValidEmail("first.last@example.com"));
}
TEST(ValidatorTest, Email_HyphenatedDomain_Valid) {
    EXPECT_TRUE(Validator::isValidEmail("user@my-domain.com"));
}
TEST(ValidatorTest, Email_Subdomain_Valid) {
    EXPECT_TRUE(Validator::isValidEmail("user@mail.example.com"));
}
TEST(ValidatorTest, Email_TwoCharTLD_Valid) {
    EXPECT_TRUE(Validator::isValidEmail("user@example.io"));
}
TEST(ValidatorTest, Email_Empty_Invalid) {
    EXPECT_FALSE(Validator::isValidEmail(""));
}
TEST(ValidatorTest, Email_NoAtSign_Invalid) {
    EXPECT_FALSE(Validator::isValidEmail("userexample.com"));
}
TEST(ValidatorTest, Email_NoLocalPart_Invalid) {
    EXPECT_FALSE(Validator::isValidEmail("@example.com"));
}
TEST(ValidatorTest, Email_NoDomainAfterAt_Invalid) {
    EXPECT_FALSE(Validator::isValidEmail("user@"));
}
TEST(ValidatorTest, Email_TLDTooShort_Invalid) {
    EXPECT_FALSE(Validator::isValidEmail("a@b.c"));
}
TEST(ValidatorTest, Email_TrailingDotOnDomain_Invalid) {
    EXPECT_FALSE(Validator::isValidEmail("user@example."));
}
TEST(ValidatorTest, Email_DomainStartsWithDot_Invalid) {
    EXPECT_FALSE(Validator::isValidEmail("user@.com"));
}
TEST(ValidatorTest, Email_SpaceInLocalPart_Invalid) {
    EXPECT_FALSE(Validator::isValidEmail("user name@example.com"));
}
TEST(ValidatorTest, Email_SpaceInDomain_Invalid) {
    EXPECT_FALSE(Validator::isValidEmail("user@example com"));
}
TEST(ValidatorTest, Email_LeadingSpace_Invalid) {
    EXPECT_FALSE(Validator::isValidEmail(" user@example.com"));
}
TEST(ValidatorTest, Email_TrailingSpace_Invalid) {
    EXPECT_FALSE(Validator::isValidEmail("user@example.com "));
}

// Password tests (min length = VALIDATOR_PASSWORD_MIN_LENGTH = 6)
TEST(ValidatorTest, Password_ExactMinLength_Valid) {
    std::string pw(Strings::VALIDATOR_PASSWORD_MIN_LENGTH, 'a');
    EXPECT_TRUE(Validator::isValidPassword(pw));
}
TEST(ValidatorTest, Password_OneBelowMinLength_Invalid) {
    std::string pw(Strings::VALIDATOR_PASSWORD_MIN_LENGTH - 1, 'a');
    EXPECT_FALSE(Validator::isValidPassword(pw));
}
TEST(ValidatorTest, Password_Empty_Invalid) {
    EXPECT_FALSE(Validator::isValidPassword(""));
}
TEST(ValidatorTest, Password_LongWithSpecialChars_Valid) {
    EXPECT_TRUE(Validator::isValidPassword("P@ssw0rd!"));
}
TEST(ValidatorTest, Password_AllDigits_Valid) {
    EXPECT_TRUE(Validator::isValidPassword("123456"));
}
TEST(ValidatorTest, Password_SingleSpace_Invalid) {
    EXPECT_FALSE(Validator::isValidPassword(" "));
}

// Username tests (implementation: !username.empty())
TEST(ValidatorTest, Username_Empty_Invalid) {
    EXPECT_FALSE(Validator::isValidUsername(""));
}
TEST(ValidatorTest, Username_SingleChar_Valid) {
    EXPECT_TRUE(Validator::isValidUsername("a"));
}
TEST(ValidatorTest, Username_NormalName_Valid) {
    EXPECT_TRUE(Validator::isValidUsername("alice123"));
}
TEST(ValidatorTest, Username_SpecialChars_Valid) {
    EXPECT_TRUE(Validator::isValidUsername("user_name-123"));
}
