<?php

// $Id$

/**
 * @file
 *
 */

/**
 * Class that holds a flat array of all FormElements.
 */
class FormElementRegistry {

  /**
   * This instance.
   * 
   * @var Registry
   */
  private static $instance = NULL;
  /**
   * A flat array of all elements
   * 
   * @var array
   */
  private $elements;
  /**
   * The clone chain of an FormElement. Strores the order in which this FormElement was cloned.
   * 
   * @var array
   */
  private $cloned;

  /**
   * Creates a singleton reference to the registry.
   * 
   * Stores a new FormElementRegistry in the form storage.
   * 
   * @staticvar Registry $instance
   * @return Registry 
   */
  public static function getInstance() {
    if (empty(self::$instance)) {
      self::$instance = new FormElementRegistry(); // Create
      module_load_include('inc', 'objective_forms', 'FormStorage');
      $storage = FormStorage::getInstance();
      $storage->elementRegistry = self::$instance; // Store
    }
    return self::$instance;
  }

  /**
   * Creates FormElementRegistry.
   */
  private function __construct() {
    $this->elements = array();
  }

  /**
   * Set the instance on wakeup.
   */
  public function __wakeup() {
    self::$instance = $this;
  }

  /**
   *
   * @param FormElement $created 
   */
  public function registerCreated(FormElement $created) {
    $this->elements[$created->hash] = $created;
  }

  /**
   *
   * @param hash $original_hash
   * @param FormElement $clone 
   */
  public function registerClone($original_hash, FormElement $clone) {
    $this->cloned[$created->hash] = $original_hash;
    $this->elements[$clone->hash] = $cloned;
  }

  public function registerDestroyed($hash) {
    // @todo Implement... Should this ever get called? Since this class holds references to every object created.
  }

  /**
   * Checks to see if the element identified by hash exists in the registry.
   * 
   * @param hash $hash 
   */
  public function exists($hash) {
    return isset($this->elements[$hash]);
  }

  /**
   * Get a registered form element identified by its hash.
   * 
   * @param hash $hash 
   * @return FormElement
   *   The form element if found otherwise NULL;  
   */
  public function get($hash) {
    if ($this->exists($hash)) {
      return $this->elements[$hash];
    }
    return NULL;
  }

  /**
   * Checks to see if the form element identified by hash is a clone of another form element.
   * 
   * @param hash $hash  
   * @return boolean
   */
  public function isClone($hash) {
    return isset($this->cloned[$hash]);
  }

  /**
   * Checks if a form element has a ancestor it was cloned from at the given depth.
   * 
   * @param hash $hash
   * @param int $depth 
   */
  public function hasAncestor($hash, $depth) {
    if ($this->isClone($hash)) {
      return isset($this->cloned[$hash][$depth]);
    }
    return FALSE;
  }

  /**
   * Gets the ancestor this form element was cloned from at the given depth in its clone history.
   * 
   * @param hash $hash
   * @param int $depth 
   *   The position in the clone history to look for the ancestor.
   * @return FormElement 
   *   The an form element this the form element was cloned from if found, otherwise NULL.
   */
  public function getAncestor($hash, $depth) {
    if ($this->hasAncestor($hash, $depth)) {
      return $this->cloned[$hash][$depth];
    }
    return NULL;
  }

  /**
   * Gets the original ancestor this form element was cloned from.
   * 
   * If the element doesn't have a clone history, return the given form element.
   * 
   * @param hash $hash 
   * @return FormElement
   */
  public function getOriginal($hash) {
    if ($this->isClone($hash)) {
      $length = count($this->cloned[$hash]);
      return $this->cloned[$hash][$length - 1];
    }
    return $this->get($hash);
  }

  /**
   * Clears the history.
   * 
   * @param hash $hash
   *   The hash of the form element whose history will be cleared. If null all history will be removed.
   */
  public function clearHistory($hash = NULL) {
    if (isset($hash)) {
      unset($this->cloned[$hash]);
    }
    else {
      $this->cloned = array();
    }
  }

}
