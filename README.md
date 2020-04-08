# Gene JBrowse Fields
This module attempts to create a field on Gene pages that will either be an iframe or a link out to an existing JBrowse instance.

It is purely a learning module. Hopefully some of this work will eventually make its way into the Tripal JBrowse module
It is currently hardcoded with the i5k site in mind, for use on an upgraded Tripal 3 version of the site. 


## Instructions
1. Install and enable this module in your normal place
2. clear the cache: ```drush cc all```
3. Navigate to the Tripal 3 Content Type administration page for Gene type
`http://i5k2.local/admin/structure/bio_data/manage/` and click on "Manage Fields"
4. Click "+ Check for new fields".  It should find the `local__jbrowse_link` field
5. On the "Manage Display" tab, drag the newly created JBrowse field to where you want it to display.

## Progress
 - [x] Get field, widget added
 - [x] Get (any) data to show up in the field
 - [x] Get desired info from database, construct iframe.
 - [ ] Remove hardcoded i5k-centric URL parameters
 - [ ] Generate secondary widget/formatter to let the admin choose how to display
    - [x] JBrowse iframe
    - [ ] Link
 - [ ] Make links dynamically generated (possible to not have to read from DB each time?)