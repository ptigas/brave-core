/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "usermodel_helper.h"

#include "chrome/browser/sessions/session_tab_helper.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_list.h"
#include "components/sessions/core/session_id.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/browser/web_contents_user_data.h"
#include "content/public/browser/navigation_entry.h"

brave_ads::UserModelHelper::UserModelHelper(content::WebContents* web_contents)
    : WebContentsObserver(web_contents),
      tab_id_(SessionTabHelper::IdForTab(web_contents)) {
  if (!tab_id_.is_valid())
    return;

  BrowserList::AddObserver(this);
}

brave_ads::UserModelHelper::~UserModelHelper() {
  BrowserList::RemoveObserver(this);
}

void brave_ads::UserModelHelper::TitleWasSet(content::NavigationEntry* entry) {
    LOG(INFO) << "Title: " << entry->GetTitle();
}

void brave_ads::UserModelHelper::DidFinishLoad(content::RenderFrameHost* render_frame_host,
                       const GURL& validated_url) {
    LOG(INFO) << "Usermodel: " << validated_url.spec();
}