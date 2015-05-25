<?php

/**
 * @file
 */

/**
 * Find Git repos in the site root, report if there are unstaged /
 * uncommitted changes.
 */
function archimedes_civicrm_get_extensions() {
  if (module_exists('civicrm')) {
    $values = array();

    civicrm_initialize();
    $mapper = CRM_Extension_System::singleton()->getMapper();
    $manager = CRM_Extension_System::singleton()->getManager();
    $extensions = array();
    $keys = array_keys($manager->getStatuses());
    foreach ($keys as $key) {
      try {
        $obj = $mapper->keyToInfo($key);
        $data = CRM_Admin_Page_Extensions::createExtendedInfo($obj);
        $node = array(
          'title' => $data['name'],
          'name' => $data['name'],
          'key' => $data['key'],
          'field_version' => $data['version'],
          'type' => $data['type'],
          'compatibility' => $data['compatibility'],
          'stage' => $data['develStage'],
          'urls' => $data['urls'],
          'status' => $data['status'],
          'release_date' => $data['releaseDate'],
          'body' => $data['description'],
        );
        $values[] = archimedes_value($node['title'], 'civicrm_extension')
          ->addNode($node);
      }
      catch (CRM_Extension_Exception $ex) {
        watchdog('archimedes_civicrm', 'Failed to read extension info for %key.', array('%key' => $key));
        continue;
      }
    }
    if (!empty($values)) {
      return $values;
    }
  }
}

class Archimedes_civicrm_extension extends Archimedes_nodereference {

  public function __construct($value) {
    $this->ns = 'monitor-plugin:civicrm-extension';
    parent::__construct($value);
  }

  public function toArray() {
    return array(
      'name' => (string) $this->value,
      'version' => $this->getAttributeNS('node:field_version'),
      'desc' => $this->getAttributeNS('node:body')
    );
  }

}
