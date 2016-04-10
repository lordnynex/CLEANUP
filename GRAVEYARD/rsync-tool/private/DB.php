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

class DB {

        /*** Declare instance ***/
        private static $instance = NULL;

        /**
        *
        * the constructor is set to private so
        * so nobody can create a new instance using new
        *
        */
        private function __construct() {
          /*** maybe set the db name here later ***/
        }

        /**
        *
        * Return DB instance or create intitial connection
        *
        * @return object (PDO)
        *
        * @access public
        *
        */
        public static function getInstance() {
                global $config;

                if(empty($config)) {
                        require(dirname(__FILE__).'/config.php');
                }

                if (!self::$instance) {
                        self::$instance = new PDO("mysql:host=".$config['dbhost'].";dbname=".$config['dbname'],
                                                $config['dbuser'], $config['dbpass']);
                        self::$instance-> setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
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
