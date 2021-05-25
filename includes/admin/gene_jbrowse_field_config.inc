<?php

/**
 * Implements hook_page_build()
 * 
 * Build a configuration page for Gene JBrowse field module
 * Configurations needed:
 *  - base URL:   "http://website.com/"
 *  - URL parameters (provide a list of placeholder variables available to the user to assemble the URL with)
 *    - assemble a string to go after the base URL: '/jbrowse/'.$genus.'%20'.$species.'$etc'
 *    - list of variables:
 *      - genus
 *      - species
 *      - common name
 *      - sequence coordinates (with or without +/-)
 *      - child sequence name
 *      - child sequence uniquename
 *      - gene name 
 *      - analysis name
 * 
 */

function gene_jbrowse_field_config_form($form, &$form_state)
{
// Build a breadcrumb
$breadcrumb = array();
$breadcrumb[] = l('Home', '<front>');
$breadcrumb[] = l('Administration', 'admin');
$breadcrumb[] = l('Tripal','admin/tripal');
$breadcrumb[] = l('Extensions', 'admin/tripal/extension');
$breadcrumb[] = l('Gene JBrowse Field', 'admin/tripal/extension/gene_jbrowse_field/config');
drupal_set_breadcrumb($breadcrumb);

// Load saved settings from the database
$query = "select name, value from gene_jbrowse_field_config";
$results = db_query($query);


if ($results->rowCount() == 0)
{
    // Set all the fields to a default if there is no info in the database
    $url_template = $mrna_url_template = "https://default.com/[genus]";
}
else
{
    while($field = $results->fetchAssoc())
    {
        $fields[$field['name']] = $field['value'];
    }
}

// Description to user of available template wildcards
$form['template_description'] = array(
    '#type'     => 'fieldset',
    '#title'    => 'URL Template Assembly Instructions',
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
    '#description'  => format_string('The following wildcards/variables are available to help you build your URL. 
    You can insert them by including [name] into the field.
    <p>
    Organism Information
    <ul>
    <li>[genus], [g]</li>
    <li>[species], [s]</li>
    <li>[common name]</li>
    </ul>
    Sequence Information
    <ul>
    <li>[coords]</li>
    <li>[gene name]</li>
    <li>[uniquename]</li>
    </ul>
    Related Information
    <ul>
    <li>[child sequence name]</li>
    <li>[child sequence uniquename]</li>
    <li>[analysis name]</li>
    </ul>
    </p>
    ')
);

// URL Template Assembly Field - Gene Pages
$form['url_template'] = array(
    '#type'         => 'textfield',
    '#title'        => 'Gene Page URL Template',
    '#size'         => '150',
    '#max_length'   => '511',
    '#description'  => 'URL Template for your JBrowse instance, to be shown on Gene pages',
    '#attributes'   => array('id' => "url_template_field"),
    // Provide a place for the JS preview script to insert the preview link. The JS must insert, via id=jbrowse_preview_link, values innerHTML and href.value (or perhaps just generate this
    //  string in its entirety in JS and just plop it here into an empty div)
    '#field_suffix' => format_string('<a href="#" target="_blank" id="jbrowse_preview_link_gene">a</a>'),
    '#default_value' => t($fields['url_template']),
);
// URL Template Assembly Field - mRNA Pages
$form['mrna_url_template'] = array(
    '#type'         => 'textfield',
    '#title'        => 'mRNA Page URL Template',
    '#size'         => '150',
    '#max_length'   => '511',
    '#description'  => 'URL Template for your JBrowse instance, to be shown on mRNA pages',
    '#attributes'   => array('id' => "mrna_url_template_field"),
    // Provide a place for the JS preview script to insert the preview link. The JS must insert, via id=jbrowse_preview_link, values innerHTML and href.value (or perhaps just generate this
    //  string in its entirety in JS and just plop it here into an empty div)
    '#field_suffix' => format_string('<a href="#" target="_blank" id="jbrowse_preview_link_mrna">b</a>'),
    '#default_value' => t($fields['mrna_url_template']),
);

// Save/submit button
$form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
);
return $form;
}

/**
 * Validate the form
 * @todo - this
 */
function gene_jbrowse_field_config_form_validate($form, &$form_state)
{

}

/**
 * Form submission
 */
function gene_jbrowse_field_config_form_submit($form, &$form_state)
{
    // Perform the same regex functions from the Javascript preview
    $url_template = $form_state['values']['url_template'];
    $mrna_url_template = $form_state['values']['mrna_url_template'];

    // Get the config_id associated to this setting
    $query = "select config_id from gene_jbrowse_field_config where name='url_template'";
    $results = db_query($query);
    $config_id = $results->fetchAssoc()[config_id];

    $query = "select config_id from gene_jbrowse_field_config where name='mrna_url_template'";
    $results = db_query($query);
    $mrna_config_id = $results->fetchAssoc()[config_id];
    // Submit to the database
    $new_value = array(
        'config_id' => $config_id,
        'name'  => 'url_template',
        'value' => $url_template,
    );

    drupal_write_record('gene_jbrowse_field_config',$new_value,'config_id');

    $new_value = array(
        'config_id' => $mrna_config_id,
        'name'  => 'mrna_url_template',
        'value' => $mrna_url_template,
    );

    drupal_write_record('gene_jbrowse_field_config',$new_value,'config_id');

    drupal_set_message("Updated the URL Template for Gene and mRNA pages.");
}