<?php
/*
*   Copyright 2012 Brandon Beveridge
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*/

require(dirname(__FILE__).'/lib/Smarty.class.php');

class SM {

        private static $instance = NULL;

        private function __construct() {
        }

        public static function getInstance() {
                if (!self::$instance) {
                        self::$instance = new Smarty;
                }

                return self::$instance;
        }

        /**
        *
        * Like the constructor, we make __clone private
        * so nobody can clone the instance
        *
        */
        private function __clone(){
        }

} /*** end of class ***/

?>
