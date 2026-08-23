#include <gtest/gtest.h>
#include "../../models/Notification.h"
#include <string>
#include <strings.h>


class NotificationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
};

TEST_F(NotificationTest, ParameterizedConstructor) {
    Notification notification(123, "Test Title", "Test Message", "Technology", 
                             "https://example.com", "Test Source", false);
    
    // Test all parameters are set correctly
    EXPECT_EQ(notification.id, 123);
    EXPECT_EQ(notification.title, "Test Title");
    EXPECT_EQ(notification.message, "Test Message");
    EXPECT_EQ(notification.category, "Technology");
    EXPECT_EQ(notification.url, "https://example.com");
    EXPECT_EQ(notification.source, "Test Source");
    EXPECT_EQ(notification.read, false);
}

TEST_F(NotificationTest, ParameterizedConstructorWithReadTrue) {
    Notification notification(456, "Read Title", "Read Message", "News", 
                             "https://news.com", "News Source", true);
    
    // Test with read = true
    EXPECT_EQ(notification.id, 456);
    EXPECT_EQ(notification.title, "Read Title");
    EXPECT_EQ(notification.message, "Read Message");
    EXPECT_EQ(notification.category, "News");
    EXPECT_EQ(notification.url, "https://news.com");
    EXPECT_EQ(notification.source, "News Source");
    EXPECT_EQ(notification.read, true);
}

TEST_F(NotificationTest, ParameterizedConstructorWithEmptyStrings) {
    Notification notification(0, "", "", "", "", "", false);
    
    // Test with empty strings
    EXPECT_EQ(notification.id, 0);
    EXPECT_EQ(notification.title, "");
    EXPECT_EQ(notification.message, "");
    EXPECT_EQ(notification.category, "");
    EXPECT_EQ(notification.url, "");
    EXPECT_EQ(notification.source, "");
    EXPECT_EQ(notification.read, false);
}

TEST_F(NotificationTest, ParameterizedConstructorWithSpecialCharacters) {
    Notification notification(789, "Title with \"quotes\" & symbols", 
                             "Message with\nnewlines\tand tabs", 
                             "Science & Technology", 
                             "https://example.com/path?param=value#fragment", 
                             "Source with spaces", true);
    
    // Test with special characters
    EXPECT_EQ(notification.id, 789);
    EXPECT_EQ(notification.title, "Title with \"quotes\" & symbols");
    EXPECT_EQ(notification.message, "Message with\nnewlines\tand tabs");
    EXPECT_EQ(notification.category, "Science & Technology");
    EXPECT_EQ(notification.url, "https://example.com/path?param=value#fragment");
    EXPECT_EQ(notification.source, "Source with spaces");
    EXPECT_EQ(notification.read, true);
}

TEST_F(NotificationTest, MultipleNotificationsIndependent) {
    Notification notification1(1, "First Title", "First Message", "News", 
                              "https://first.com", "First Source", false);
    Notification notification2(2, "Second Title", "Second Message", "Sports", 
                              "https://second.com", "Second Source", true);
    
    // Verify they are independent
    EXPECT_EQ(notification1.id, 1);
    EXPECT_EQ(notification1.title, "First Title");
    EXPECT_EQ(notification1.message, "First Message");
    EXPECT_EQ(notification1.category, "News");
    EXPECT_EQ(notification1.read, false);
    
    EXPECT_EQ(notification2.id, 2);
    EXPECT_EQ(notification2.title, "Second Title");
    EXPECT_EQ(notification2.message, "Second Message");
    EXPECT_EQ(notification2.category, "Sports");
    EXPECT_EQ(notification2.read, true);
}

TEST_F(NotificationTest, DirectMemberAccess) {
    Notification notification(999, "Original Title", "Original Message", "Original Category", 
                             "https://original.com", "Original Source", false);
    
    // Test direct member access and modification
    notification.id = 1000;
    notification.title = "Modified Title";
    notification.message = "Modified Message";
    notification.category = "Modified Category";
    notification.url = "https://modified.com";
    notification.source = "Modified Source";
    notification.read = true;
    
    // Verify modifications
    EXPECT_EQ(notification.id, 1000);
    EXPECT_EQ(notification.title, "Modified Title");
    EXPECT_EQ(notification.message, "Modified Message");
    EXPECT_EQ(notification.category, "Modified Category");
    EXPECT_EQ(notification.url, "https://modified.com");
    EXPECT_EQ(notification.source, "Modified Source");
    EXPECT_EQ(notification.read, true);
}

TEST_F(NotificationTest, EdgeCases) {
    Notification notification(0, "", "", "", "", "", false);
    
    // Test with very large ID
    notification.id = 2147483647; // Max int
    EXPECT_EQ(notification.id, 2147483647);
    
    // Test with negative ID
    notification.id = -2147483648; // Min int
    EXPECT_EQ(notification.id, -2147483648);
    
    // Test with very long strings
    std::string longString = "This is a very long string with many characters to test the limits of the string storage in the Notification struct. It should handle long strings without any issues.";
    notification.title = longString;
    notification.message = longString;
    notification.category = longString;
    notification.url = longString;
    notification.source = longString;
    
    EXPECT_EQ(notification.title, longString);
    EXPECT_EQ(notification.message, longString);
    EXPECT_EQ(notification.category, longString);
    EXPECT_EQ(notification.url, longString);
    EXPECT_EQ(notification.source, longString);
}

TEST_F(NotificationTest, CopyConstructor) {
    Notification original(123, "Original Title", "Original Message", "Original Category", 
                         "https://original.com", "Original Source", false);
    
    Notification copy = original;
    
    // Verify copy has same values
    EXPECT_EQ(copy.id, original.id);
    EXPECT_EQ(copy.title, original.title);
    EXPECT_EQ(copy.message, original.message);
    EXPECT_EQ(copy.category, original.category);
    EXPECT_EQ(copy.url, original.url);
    EXPECT_EQ(copy.source, original.source);
    EXPECT_EQ(copy.read, original.read);
    
    // Verify they are independent
    copy.id = 456;
    copy.title = "Modified Title";
    copy.read = true;
    EXPECT_NE(copy.id, original.id);
    EXPECT_NE(copy.title, original.title);
    EXPECT_NE(copy.read, original.read);
}

TEST_F(NotificationTest, AssignmentOperator) {
    Notification original(123, "Original Title", "Original Message", "Original Category", 
                         "https://original.com", "Original Source", false);
    Notification assigned(0, "", "", "", "", "", true);
    
    assigned = original;
    
    // Verify assignment
    EXPECT_EQ(assigned.id, original.id);
    EXPECT_EQ(assigned.title, original.title);
    EXPECT_EQ(assigned.message, original.message);
    EXPECT_EQ(assigned.category, original.category);
    EXPECT_EQ(assigned.url, original.url);
    EXPECT_EQ(assigned.source, original.source);
    EXPECT_EQ(assigned.read, original.read);
}

TEST_F(NotificationTest, BooleanReadField) {
    // Test read field with different boolean values
    Notification unread(1, "Unread", "Message", "Category", "https://example.com", "Source", false);
    Notification read(2, "Read", "Message", "Category", "https://example.com", "Source", true);
    
    EXPECT_EQ(unread.read, false);
    EXPECT_EQ(read.read, true);
    
    // Test toggling read status
    unread.read = true;
    read.read = false;
    
    EXPECT_EQ(unread.read, true);
    EXPECT_EQ(read.read, false);
}

TEST_F(NotificationTest, NotificationWithLongMessage) {
    std::string longMessage = "This is a very long notification message that contains a lot of text. "
                              "It should be able to handle messages of considerable length without any issues. "
                              "The message might contain multiple sentences and various types of content.";
    
    Notification notification(123, "Long Message Title", longMessage, "Category", 
                             "https://example.com", "Source", false);
    
    EXPECT_EQ(notification.message, longMessage);
    EXPECT_EQ(notification.message.length(), longMessage.length());
}
