# Gene JBrowse Fields
This module creates a field on Gene pages that features a JBrowse instance embedded in an iframe

The URL of the JBrowse instance in the iframe is configurable to fit most URL schemas. *The JBrowse instance must already exist.*


## Instructions
#### Installation
1. Install and enable this module in your normal place
2. clear the cache: ```drush cc all```
#### Configuration
3. Navigate to `admin/tripal/extension/gene_jbrowse_field/config` in the browser to construct the URL. Use the tokens provided to ensure that the URL actually resolves to your JBrowse instance.

#### Enable Field
4. Navigate to the Tripal 3 Content Type administration page for Gene type
`/admin/structure/bio_data/manage/` and click on "Manage Fields"
5. Click "+ Check for new fields".  It should find the `local__jbrowse_link` field
6. On the "Manage Display" tab, drag the newly created JBrowse field to where you want it to display.

## Progress/Task List
 - [x] Get field, widget added
 - [x] Get (any) data to show up in the field
 - [x] Get desired info from database, construct iframe.
 - [x] Remove hardcoded i5k-centric URL parameters
 - [ ] Generate secondary widget/formatter to let the admin choose how to display
    - [x] JBrowse iframe
    - [ ] Link
 - [x] Make links dynamically generated (possible to not have to read from DB each time?)
 - [ ] mRNA Page