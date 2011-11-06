/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



package ifc.ui.dialogs;

import lib.MultiMethodTest;

import com.sun.star.ui.dialogs.XFilterManager;

/**
* Testing <code>com.sun.star.ui.XFilterManager</code>
* interface methods :
* <ul>
*  <li><code> appendFilter()</code></li>
*  <li><code> setCurrentFilter()</code></li>
*  <li><code> getCurrentFilter()</code></li>
* </ul> <p>
* Test is <b> NOT </b> multithread compilant. <p>
* @see com.sun.star.ui.XFilterManager
*/
public class _XFilterManager extends MultiMethodTest {

    public XFilterManager oObj = null;

    /**
    * Appends a new filter (for extension 'txt'). <p>
    * Has <b>OK</b> status if no runtime exceptions ocured.
    */
    public void _appendFilter() {
        boolean res = true;
        try {
            oObj.appendFilter("TestFilter", "txt");
        } catch (com.sun.star.lang.IllegalArgumentException e) {
            e.printStackTrace(log);
            res=false;
        }

        tRes.tested("appendFilter()", res) ;
    }

    /**
    * Sets the current filter to that which was appended before.<p>
    * Has <b>OK</b> status if no exceptions occured, else one of
    * <code>appendFilter</code> and <code>setCurrentFilter</code>
    * methods failed. <p>
    * The following method tests are to be completed successfully before :
    * <ul>
    *  <li> <code> appendFilter </code>  </li>
    * </ul>
    */
    public void _setCurrentFilter() {
        requiredMethod("appendFilter()") ;
        boolean result = true;

        try {
            oObj.setCurrentFilter("TestFilter") ;
        } catch (com.sun.star.lang.IllegalArgumentException e) {
            log.println("setCurrentFilter() or appendFilter() failed") ;
            result = false ;
        }

        tRes.tested("setCurrentFilter()", result) ;
    }

    /**
    * Gets current filter name and compares it filter name set before.<p>
    * Has <b>OK</b> status if set and get filter names are equal.<p>
    * The following method tests are to be completed successfully before :
    * <ul>
    *  <li> <code> setCurrentFilter </code> </li>
    * </ul>
    */
    public void _getCurrentFilter() {
        requiredMethod("setCurrentFilter()") ;

        String gVal = oObj.getCurrentFilter() ;

        tRes.tested("getCurrentFilter()", "TestFilter".equals(gVal)) ;
    }
}


