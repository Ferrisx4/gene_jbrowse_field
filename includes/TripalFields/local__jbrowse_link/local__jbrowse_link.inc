<?php
/**
 * @class
 * Purpose:
 *
 * Data:
 * Assumptions:
 */
class local__jbrowse_link extends ChadoField {

  // --------------------------------------------------------------------------
  //                     EDITABLE STATIC CONSTANTS
  //
  // The following constants SHOULD be set for each descendant class.  They are
  // used by the static functions to provide information to Drupal about
  // the field and it's default widget and formatter.
  // --------------------------------------------------------------------------

  // The default label for this field.
  public static $default_label = 'JBrowse';

  // The default description for this field.
  public static $default_description = 'Link to a JBrowse instance';

  // The default widget for this field.
  public static $default_widget = 'local__jbrowse_link_widget';

  // The default formatter for this field.
  public static $default_formatter = 'local__jbrowse_link_formatter';

  // The module that manages this field.
  // If no module manages the field (IE it's added via libraries)
  // set this to 'tripal_chado'
  public static $module = 'gene_jbrowse_field';

  // A list of global settings. These can be accessed within the
  // globalSettingsForm.  When the globalSettingsForm is submitted then
  // Drupal will automatically change these settings for all fields.
  // Once instances exist for a field type then these settings cannot be
  // changed.
  public static $default_settings = array(
    'storage' => 'field_chado_storage',
     // It is expected that all fields set a 'value' in the load() function.
     // In many cases, the value may be an associative array of key/value pairs.
     // In order for Tripal to provide context for all data, the keys should
     // be a controlled vocabulary term (e.g. rdfs:type). Keys in the load()
     // function that are supported by the query() function should be
     // listed here.
     'searchable_keys' => array(),
  );

  // Indicates the download formats for this field.  The list must be the
  // name of a child class of the TripalFieldDownloader.
  public static $download_formatters = array(
     'TripalTabDownloader',
     'TripalCSVDownloader',
  );

  // Provide a list of instance specific settings. These can be access within
  // the instanceSettingsForm.  When the instanceSettingsForm is submitted
  // then Drupal with automatically change these settings for the instance.
  // It is recommended to put settings at the instance level whenever possible.
  // If you override this variable in a child class be sure to replicate the
  // term_name, term_vocab, term_accession and term_fixed keys as these are
  // required for all TripalFields.
  public static $default_instance_settings = array(
    // The DATABASE name, as it appears in chado.db.  This also builds the link-out url.  In most cases this will simply be the CV name.  In some cases (EDAM) this will be the SUBONTOLOGY.
    'term_vocabulary' => 'local',
    // The name of the term.
    'term_name' => 'jbrowse_link',
    // The unique ID (i.e. accession) of the term.
    'term_accession' => 'jbrowse',
    // Set to TRUE if the site admin is not allowed to change the term
    // type, otherwise the admin can change the term mapped to a field.
    'term_fixed' => FALSE,
    // Indicates if this field should be automatically attached to display
    // or web services or if this field should be loaded separately. This
    // is convenient for speed.  Fields that are slow should for loading
    // should have auto_attach set to FALSE so tha their values can be
    // attached asynchronously.
    'auto_attach' => FALSE,
    // The table in Chado that the instance maps to.
    'chado_table' => '',
    // The column of the table in Chado where the value of the field comes from.
    'chado_column' => '',
    // The base table.
    'base_table' => '',
  );

  // A boolean specifying that users should not be allowed to create
  // fields and instances of this field type through the UI. Such
  // fields can only be created programmatically with field_create_field()
  // and field_create_instance().
  public static $no_ui = FALSE;

  // A boolean specifying that the field will not contain any data. This
  // should exclude the field from web services or downloads.  An example
  // could be a quick search field that appears on the page that redirects
  // the user but otherwise provides no data.
  public static $no_data = FALSE;

   /**
    * @see ChadoField::load()
    *
    **/

  public function load($entity) {

    // ChadoFields automatically load the chado column specified in the
    // default settings above. If that is all you need then you don't even
    // need to implement this function. However, if you need to add any
    // additional data to be used in the display, you should add it here.
    parent::load($entity);
    $field = get_class($this);

    // Get the record
    $record = $entity->chado_record;

    /**
     * Gather the necessary information to construct a link
     * Example:
     * https://apollo.nal.usda.gov/apollo/Oncopeltus%20fasciatus/jbrowse/?loc=Scaffold25%3A333819..335126
    */

    // Hardcode the URL for now
    //$base_url = "https://apollo.nal.usda.gov/apollo/";
    // Get the URL template
    // Load saved settings from the database
    $bundle_type = $record->type_id->name;
    if ($bundle_type == 'gene')
    {
      $query = "select name, value from gene_jbrowse_field_config where name='url_template'";
    }
    elseif ($bundle_type == 'mRNA')
    {
      $query = "select name, value from gene_jbrowse_field_config where name='mrna_url_template'";
    }
    else
    {
      dpm("not gene or mrna");
    }
    $results = db_query($query);

    if ($results->rowCount() == 0)
    {
        $url_template = "https://";
    }
    else
    {
        $url_template = $results->fetchAssoc()[value];
    }
    // Get the genus, species, and common name of the organism
    if ($record)
    {
      // genus, species, common name
      $organism = $record->organism_id;

      $genus  = $organism->genus;
      $species = $organism->species;
      $common_name = $organism->common_name;
    }
    else
    {
      dpm("Could not get the record");
      return;
    }

    // Get the gene location info 
    $options = [
      'return_array' => TRUE,
      'order_by' => ['rank' => 'ASC'],
    ];
    $feature = chado_expand_var($record, 'table', 'featureloc', $options);
    
    $aligned = $feature->featureloc->feature_id;
    if ($aligned) {
      foreach ($aligned as $index => $featureloc) {
        $srcfeature = $featureloc->srcfeature_id->name;
        $strand = '';
        if ($featureloc->strand == 1) {
          $strand = '+';
        }
        elseif ($featureloc->strand == -1) {
          $strand = '-';
        }
        else {
          $strand = '';
        }
        $fmin = $featureloc->fmin + 1;
        $fmax = $featureloc->fmax;
        $seq_coords = $srcfeature . ':' . $fmin . '..' . $fmax . $strand;
      }
    }

    // Get the analysis object and related data
    $id = $record->feature_id;
    $analyses = chado_query('SELECT * FROM {analysisfeature} 
                              WHERE feature_id = :id', [
      ':id' => $id,
    ])->fetchAll();
    foreach($analyses as $analysis)
    {
      $analysis_entity_id = chado_get_record_entity_by_table('analysis', $analysis->analysis_id);
    }
    $analysis_title = db_query('SELECT title FROM {tripal_entity} WHERE id=:id', [':id' => $analysis_entity_id])->fetchField();

    // Get information of the children.
    // @TODO - are there genes that have multiple children, and if so, how do we deal with them?
    $child_finder_sql = "
      SELECT F.name,F.uniquename
      FROM {feature_relationship} FR
        INNER JOIN {feature} F on FR.subject_id = F.feature_id
        INNER JOIN {cvterm} CVT on CVT.cvterm_id = F.type_id
        INNER JOIN {cvterm} RCVT on RCVT.cvterm_id = FR.type_id
      WHERE
        FR.object_id = :id 
      ORDER BY FR.rank ASC";
    $children = chado_query($child_finder_sql, [':id' => $id]);
    $child = $children->fetchObject();

    /**
     * Assemble the array of all available tokens and their real values
     */
    // Associative array of basic template tokens
    $toks = array(
      'genus'         => strtolower($genus),
      'species'       => $species,
      'common name'   => $common_name,
      'coords'        => $seq_coords,
      'child sequence name' => $child->name,
      'child sequence uniquename' => $child->uniquename,
      'gene name'     => $record->name,
      'analysis name' => $analysis_title,
      'uniquename'   => $record->uniquename,
    );
    // Expand the array with special CaSeS and such
    $toks['Genus']      = ucfirst($toks[genus]);
    $toks['Species']    = ucfirst($toks[species]);
    $toks['g']          = strtolower(substr($toks[genus],0,1));
    $toks['s']          = substr($toks[species],0,1);
    $toks['G']          = substr($toks[Genus],0,1);
    $toks['S']          = substr($toks[Species],0,1);


    foreach($toks as $key => $token)
    {
        $pattern = '/\['.$key.'\]/';
        $url_template = preg_replace($pattern,$token,$url_template);
    }
    // All together now!
    //$ext_jb_url = $base_url.$genus.'%20'.$species.'/jbrowse/?loc='.$seq_coords.'&tracks=DNA%2CAnnotations%2C'.$genus_code.$species_code.'_current_models';

    $entity->{$field}['und'][0]['value'] = $url_template;
  }



    /**
    * @see ChadoField::query()
    *
    **/

  public function query($query, $condition) {
  }

    /**
    * @see ChadoField::queryOrder()
    *
    **/

  public function queryOrder($query, $order) {
  }


    /**
    * @see ChadoField::elementInfo()
    *
    **/
    
  public function elementInfo() {
    $field_term = $this->getFieldTermID();
    return array(
      $field_term => array(
        'operations' => array('eq', 'ne', 'contains', 'starts'),
        'sortable' => TRUE,
        'searchable' => TRUE,
      ),
    );
  }

  /**
   * Fetch all the relevant data
   */
  public function get_chado_data()
  {

  }
}


