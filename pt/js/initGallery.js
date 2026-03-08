/*
 * blueimp Gallery Demo JS
 * https://github.com/blueimp/Gallery
 *
 * Copyright 2013, Sebastian Tschan
 * https://blueimp.net
 *
 * Licensed under the MIT license:
 * https://opensource.org/licenses/MIT
 */

/* global blueimp, $ */

$(function () {
  'use strict'
  // Initialize the Gallery as video carousel:
  // eslint-disable-next-line new-cap
  blueimp.Gallery(document.getElementById('links').getElementsByTagName('a'), {
    //container: '#__docusaurus div div #blueimp-gallery-carousel',
    container: document.getElementById('blueimp-gallery-carousel'),
    carousel: true
  })
})
