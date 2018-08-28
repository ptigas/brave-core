/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <vector>

#include "chrome/browser/ui/browser.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "content/public/browser/navigation_entry.h"
#include "content/public/test/browser_test_utils.h"

using BraveContentBrowserClientTest = InProcessBrowserTest;

IN_PROC_BROWSER_TEST_F(BraveContentBrowserClientTest, CanLoadChromeURL) {
  GURL chrome_settings_url("chrome://settings/");
  std::vector<GURL> urls {
    chrome_settings_url,
    GURL("about:settings/"),
    GURL("brave://settings/")
  };
  std::for_each(urls.begin(), urls.end(), [this, &chrome_settings_url](const GURL& url) {
    content::WebContents* contents = browser()->tab_strip_model()->GetActiveWebContents();
    ui_test_utils::NavigateToURL(browser(), url);
    ASSERT_TRUE(WaitForLoadStop(contents));
    EXPECT_STREQ(contents->GetLastCommittedURL().spec().c_str(),
        chrome_settings_url.spec().c_str());
  });
}

IN_PROC_BROWSER_TEST_F(BraveContentBrowserClientTest, CanLoadCustomBravePages) {
  std::vector<GURL> urls {
    GURL("chrome://adblock/"),
    GURL("chrome://rewards/"),
    GURL("chrome://welcome/")
  };
  std::for_each(urls.begin(), urls.end(), [this](const GURL& url) {
    content::WebContents* contents = browser()->tab_strip_model()->GetActiveWebContents();
    ui_test_utils::NavigateToURL(browser(), url);
    ASSERT_TRUE(WaitForLoadStop(contents));
    EXPECT_STREQ(contents->GetLastCommittedURL().spec().c_str(),
        url.spec().c_str());
  });
}

IN_PROC_BROWSER_TEST_F(BraveContentBrowserClientTest, RewriteMagnetURL) {
  content::WebContents* contents = browser()->tab_strip_model()->GetActiveWebContents();
  GURL url("magnet:?xt=urn:btih:dd8255ecdc7ca55fb0bbf81323d87062db1f6d1c&dn=Big+Buck+Bunny&tr=udp%3A%2F%2Fexplodie.org%3A6969&tr=udp%3A%2F%2Ftracker.coppersurfer.tk%3A6969&tr=udp%3A%2F%2Ftracker.empire-js.us%3A1337&tr=udp%3A%2F%2Ftracker.leechers-paradise.org%3A6969&tr=udp%3A%2F%2Ftracker.opentrackr.org%3A1337&tr=wss%3A%2F%2Ftracker.btorrent.xyz&tr=wss%3A%2F%2Ftracker.fastcast.nz&tr=wss%3A%2F%2Ftracker.openwebtorrent.com&ws=https%3A%2F%2Fwebtorrent.io%2Ftorrents%2F&xs=https%3A%2F%2Fwebtorrent.io%2Ftorrents%2Fbig-buck-bunny.torrent");
  GURL extension_url("chrome-extension://lgjmpdmojkpocjcopdikifhejkkjglho/extension/brave_webtorrent.html?magnet%3A%3Fxt%3Durn%3Abtih%3Add8255ecdc7ca55fb0bbf81323d87062db1f6d1c%26dn%3DBig%2BBuck%2BBunny%26tr%3Dudp%253A%252F%252Fexplodie.org%253A6969%26tr%3Dudp%253A%252F%252Ftracker.coppersurfer.tk%253A6969%26tr%3Dudp%253A%252F%252Ftracker.empire-js.us%253A1337%26tr%3Dudp%253A%252F%252Ftracker.leechers-paradise.org%253A6969%26tr%3Dudp%253A%252F%252Ftracker.opentrackr.org%253A1337%26tr%3Dwss%253A%252F%252Ftracker.btorrent.xyz%26tr%3Dwss%253A%252F%252Ftracker.fastcast.nz%26tr%3Dwss%253A%252F%252Ftracker.openwebtorrent.com%26ws%3Dhttps%253A%252F%252Fwebtorrent.io%252Ftorrents%252F%26xs%3Dhttps%253A%252F%252Fwebtorrent.io%252Ftorrents%252Fbig-buck-bunny.torrent");

  ui_test_utils::NavigateToURL(browser(), url);
  ASSERT_TRUE(WaitForLoadStop(contents));
  EXPECT_STREQ(contents->GetLastCommittedURL().spec().c_str(),
      url.spec().c_str()) << "URL visible to users should stay as the magnet URL";
  content::NavigationEntry* entry = contents->GetController().GetLastCommittedEntry();
  EXPECT_STREQ(entry->GetURL().spec().c_str(),
      extension_url.spec().c_str()) << "Real URL should be extension URL";
}
