<?php
/** 
 * function webform_edit_select
 * Create a set of form items to be displayed on the form for editing this component.
 * Use care naming the form items, as this correlates directly to the database schema.
 * The component "Name" and "Description" fields are added to every component type and
 * are not necessary to specify here (although they may be overridden if desired).
 * @returns An array of form items to be displayed on the edit component page
 **/
function _webform_edit_select ($currfield) {
    $editFields = array();
    $editFields['extra']['items'] = array (
      '#type' => 'textarea',
      '#title' => t("Options"),
      '#default_value' => $currfield['extra']['items'],
      '#description' => t('A list of selectable options. One option per line.').'<br />'.webform_help('webform/helptext#variables'),
      '#cols' => 60,
      '#rows' => 5,
      '#weight' => -2,
      '#required' => TRUE,
    );
    $editFields['value'] = array (
      '#type' => 'textfield',
      '#title' => t("Default value"),
      '#default_value' => $currfield['default'],
      '#description' => t('The default value of the field.').'<br />'.webform_help('webform/helptext#variables'),
      '#size' => 60,
      '#maxlength' => 127,
      '#weight' => 0,
    );
    $editFields['extra']['multiple'] = array (
      '#type' => 'checkbox',
      '#title' => t("Multiple"),
      '#return_value' => 'Y',
      '#default_value' => ($currfield['extra']['multiple']=='Y'?TRUE:FALSE),
      '#description' => t('Check this option if the user should be allowed to choose multiple values.'),
    );
    $editFields['extra']['aslist'] = array (
      '#type' => 'checkbox',
      '#title' => t("Listbox"),
      '#return_value' => 'Y',
      '#default_value' => ($currfield['extra']['aslist']=='Y'?TRUE:FALSE),
      '#description' => t('Check this option if you want the select component to be of listbox type instead of radiobuttons or checkboxes.'),
    );
    return $editFields;
}

/** 
 * function webform_render_select
 * Build a form item array containing all the properties of this component
 * @param $component An array of information describing the component, directly correlating to the webform_component database schema
 * @returns An array of a form item to be displayed on the client-side webform
 **/
function _webform_render_select ($component) {
	$formItem = array (
    '#title'         => $component['name'],
    '#required'      => $component['mandatory'],
    '#weight'        => $component['weight'],
    '#description'   => _webform_filtervalues($component['extra']['description']),
	);
	
	// Convert the user-entered list into an array
	$value = _webform_filtervalues($component['value']);
	$items = explode("\n", _webform_filtervalues($component['extra']['items']));
	$options = array();
	foreach($items as $k => $v) {
		$v = trim($v);
		if ( strlen($v) )
			$options[$v] = $v;
	}
	// Set the component options
	$formItem['#options'] = $options;
	
	if ( $component['extra']['aslist'] == 'Y' ) {
		// Set display as a select list:
		$formItem['#default_value'] = array($value);
		$formItem['#type'] = 'select';
		if ( $component['extra']['multiple'] == 'Y' ) {
			$formItem['#multiple'] = TRUE;
		}
	} else {
		if ( $component['extra']['multiple'] == 'Y' ) {
			// Set display as a checkbox set
			$formItem['#default_value'] = array($value);
			$formItem['#type'] = 'checkboxes';
			
		} else {
			// Set display as a radio set
			$formItem['#default_value'] = $value;
			$formItem['#type'] = 'radios';
		}
	}
	return $formItem;
}


/** 
 * function _webform_submission_display_select
 * Display the result of a textfield submission. The output of this function will be displayed under the "results" tab then "submissions"
 * @param $data An array of information containing the submission result, directly correlating to the webform_submitted_data database schema
 * @param $component An array of information describing the component, directly correlating to the webform_component database schema
 * @returns Textual output formatted for human reading.
 **/
function _webform_submission_display_select ($data,$component) {
  $formItem = _webform_render_select($component);
	if ($component['extra']['multiple'] == 'Y') {
		// Set the value as an array
		$formItem['#default_value'] = $data['value'];
	} else {
    // Set the value as a single string
    foreach ($data['value'] as $value) {
      if ($value !== '0') {
        $formItem['#default_value'] = $value;
        break;
      }
    }
	}
  $formItem['#attributes'] = array("disabled" => "disabled");
  return $formItem;
}

/** 
 * function _webform_analysis_view_select
 * Calculate and returns statistics about results for this component from all submission to this webform. The output of this function will be displayed under the "results" tab then "analysis"
 * @param $component An array of information describing the component, directly correlating to the webform_component database schema
 * @returns An array of data rows, each containing a statistic for this component's submissions.
 **/
function _webform_analysis_rows_select ($component) {
    $query = 'SELECT data, count(data) as datacount '.
      ' FROM {webform_submitted_data} '.
      ' WHERE nid = %d '.
      ' AND cid = %d '.
      ' AND data != "0" '.
      ' GROUP BY data ';
    $result = db_query($query, $component['nid'], $component['cid']);
    $rows = array();
    while ($data = db_fetch_array($result)) {
      $rows[] = array($data['data'], $data['datacount']);
    }
    return $rows;
}

/** 
 * function _webform_table_data_select
 * Return the result of this component's submission for display in a table. The output of this function will be displayed under the "results" tab then "table"
 * @param $data An array of information containing the submission result, directly correlating to the webform_submitted_data database schema
 * @returns Textual output formatted for human reading.
 **/
function _webform_table_data_select ($data) {
  // Set the value as a single string
  if (is_array($data['value'])) {
    foreach ($data['value'] as $value) {
      if ($value !== '0') {
        $output .= $value."<br />";
      }
    }
  } else {
    $output = empty($data['value']['0']) ? "" : $data['value']['0'];
  }
  return $output;
}


/** 
 * function _webform_csv_headers_select
 * Return the header information for this component to be displayed in a comma seperated value file. The output of this function will be displayed under the "results" tab then "download"
 * @param $component An array of information describing the component, directly correlating to the webform_component database schema
 * @returns An array of data to be displayed in the first three rows of a CSV file, not including either prefixed or trailing commas
 **/
function _webform_csv_headers_select ($component) {
  $header = array();
  $header[0] = '';
  $header[1] = $component['name'];
  
  $items = split("\n", $component['extra']['items']);
  foreach ($items as $item) {
    $item = trim($item);
    $header[2] .= ",".$item;
  }
  // Remove the preceding extra comma
  $header[2] = substr($header[2],1);
  return $header;
}

/** 
 * function _webform_csv_data_select
 * Return the result of a textfield submission. The output of this function will be displayed under the "results" tab then "submissions"
 * @param $data An array of information containing the submission result, directly correlating to the webform_submitted_data database schema
 * @returns Textual output formatted for CSV, not including either prefixed or trailing commas
 **/
function _webform_csv_data_select ($data,$component) {
  $items = split("\n", $component['extra']['items']);
  foreach($items as $item) {
    $item = trim($item);
    if (in_array($item,(array)$data['value']) === true) {
      $output .= ',Yes';
    } else {
      $output .= ',No';
    }
  }
  // Remove the preceding extra comma
  $output = substr($output,1);
  return $output;
}
?>
