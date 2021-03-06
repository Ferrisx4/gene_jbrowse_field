<?php
/**
 * @class
 * Purpose:
 *
 * Display:
 * Configuration:
 */
class local__jbrowse_link_formatter extends ChadoFieldFormatter {

  // The default label for this field.
  public static $default_label = 'JBrowse';

  // The list of field types for which this formatter is appropriate.
  public static $field_types = array('local__jbrowse_link');

  // The list of default settings for this formatter.
  public static $default_settings = array(
    'setting1' => 'default_value',
  );

   /**
    * @see ChadoFieldFormatter::settingsForm()
    *
    **/

  public function settingsForm($view_mode, $form, &$form_state) {

  }

    /**
    * @see ChadoFieldFormatter::View()
    *
    **/

  public function view(&$element, $entity_type, $entity, $langcode, $items, $display) {

    // Get the settings
    $settings = $display['settings'];
    $field = 'local__jbrowse_link';

    //dpm($entity->{$field}['und'][0]['value']);
    //$output = $entity->{$field}['und'][0]['value'];

    $output = $entity->{$field}['und'][0]['value'];
    // 508 Compliance section
    $iframe_title = "Embedded JBrowse Viewer for " . $entity->chado_record->name;
    $iframe_description = "The below embedded viewer is not fully accessible, 
                            however you can view the coordinate data in the 'Sequence 
                            Coordinates' field in the Summary tab on this page.";
    $iframe_code = '<p id="iframe_description" class="sr_only">' . $iframe_description . '</p>';
    $iframe_code .= '<iframe width="100%" height="500px" title="' . $iframe_title . '" src="'.$output.'"></iframe>';
    // for testing without burdening the remote resource: $iframe_code = '<a id="iframe_code" target="_blank" href="'.$output.'">The JBrowse (opens in new tab): '.$output.'</a>';
      $element[] = [
        '#type' => 'markup',
        '#markup' => $iframe_code,
      ];
  }

    /**
    * @see ChadoFieldFormatter::settingsSummary()
    *
    **/

  public function settingsSummary($view_mode) {
    return '';
  }

}