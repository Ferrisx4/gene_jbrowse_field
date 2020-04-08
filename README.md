# Gene JBrowse Fields
This module attempts to create a field on Gene pages that will either be an iframe or a link out to an existing JBrowse instance.

It is purely a learning module. Hopefully some of this work will eventually make its way into the Tripal JBrowse module
It is currently hardcoded with the i5k site in mind, for use on an upgraded Tripal 3 version of the site. 

## Progress
 - [x] Get field, widget added
 - [x] Get (any) data to show up in the field
 - [x] Get desired info from database, construct iframe.
 - [ ] Remove hardcoded i5k-centric URL parameters
 - [ ] Generate secondary widget/formatter to let the admin choose how to display
    - [x] JBrowse iframe
    - [ ] Link
 - [ ] Make links dynamically generated (possible to not have to read from DB each time?)