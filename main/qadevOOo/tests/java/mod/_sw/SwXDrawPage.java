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



package mod._sw;

import java.io.PrintWriter;

import lib.StatusException;
import lib.TestCase;
import lib.TestEnvironment;
import lib.TestParameters;
import util.InstCreator;
import util.SOfficeFactory;
import util.ShapeDsc;

import com.sun.star.drawing.XDrawPage;
import com.sun.star.drawing.XDrawPageSupplier;
import com.sun.star.drawing.XShapes;
import com.sun.star.lang.XMultiServiceFactory;
import com.sun.star.text.XTextDocument;
import com.sun.star.uno.UnoRuntime;
import com.sun.star.uno.XInterface;

/**
 * Test for object which is represented by service
 * <code>com.sun.star.drawing.DrawPage</code>. <p>
 * Object implements the following interfaces :
 * <ul>
 *  <li> <code>com::sun::star::drawing::GenericDrawPage</code></li>
 *  <li> <code>com::sun::star::drawing::XShapeBinder</code></li>
 *  <li> <code>com::sun::star::drawing::XShapeGrouper</code></li>
 *  <li> <code>com::sun::star::drawing::XShapes</code></li>
 *  <li> <code>com::sun::star::lang::XServiceInfo</code></li>
 *  <li> <code>com::sun::star::beans::XPropertySet</code></li>
 *  <li> <code>com::sun::star::container::XIndexAccess</code></li>
 *  <li> <code>com::sun::star::container::XElementAccess</code></li>
 *  <li> <code>com::sun::star::drawing::XMasterPageTarget</code></li>
 *  <li> <code>com::sun::star::container::XNamed</code></li>
 *  <li> <code>com::sun::star::drawing::XShapeCombiner</code></li>
 * </ul> <p>
 * This object test <b> is NOT </b> designed to be run in several
 * threads concurently.
 * @see com.sun.star.drawing.GenericDrawPage
 * @see com.sun.star.drawing.XShapeBinder
 * @see com.sun.star.drawing.XShapeGrouper
 * @see com.sun.star.drawing.XShapes
 * @see com.sun.star.lang.XServiceInfo
 * @see com.sun.star.beans.XPropertySet
 * @see com.sun.star.container.XIndexAccess
 * @see com.sun.star.container.XElementAccess
 * @see com.sun.star.drawing.XMasterPageTarget
 * @see com.sun.star.container.XNamed
 * @see com.sun.star.drawing.XShapeCombiner
 * @see ifc.drawing._GenericDrawPage
 * @see ifc.drawing._XShapeBinder
 * @see ifc.drawing._XShapeGrouper
 * @see ifc.drawing._XShapes
 * @see ifc.lang._XServiceInfo
 * @see ifc.beans._XPropertySet
 * @see ifc.container._XIndexAccess
 * @see ifc.container._XElementAccess
 * @see ifc.drawing._XMasterPageTarget
 * @see ifc.container._XNamed
 * @see ifc.drawing._XShapeCombiner
 */
public class SwXDrawPage extends TestCase {
    XTextDocument xTextDoc;

    /**
    * Creates text document.
    */
    protected void initialize( TestParameters tParam, PrintWriter log ) {
        SOfficeFactory SOF = SOfficeFactory.getFactory( (XMultiServiceFactory)tParam.getMSF() );
        try {
            log.println( "creating a textdocument" );
            xTextDoc = SOF.createTextDoc( null );
        } catch ( com.sun.star.uno.Exception e ) {
            e.printStackTrace( log );
            throw new StatusException( "Couldn't create document", e );
        }
    }

    /**
    * Disposes text document.
    */
    protected void cleanup( TestParameters tParam, PrintWriter log ) {
        log.println( "    disposing xTextDoc " );
        util.DesktopTools.closeDoc(xTextDoc);
    }

    /**
    * Creating a Testenvironment for the interfaces to be tested. After
    * obtaining DrawPage object several shapes are inserted to it.
    *     Object relations created :
    * <ul>
    *  <li> <code>'Shape'</code> for
    *      {@link ifc.drawing._XShapes} : shape creator. </li>
    *  <li> <code>'DrawPage'</code> for
    *      {@link ifc.drawing._XShapeBinder},
    *      {@link ifc.drawing._XShapeCombiner},
    *      {@link ifc.drawing._XShapeGrouper} : DrawPage component we are
    *      testing.</li>
    * </ul>
    */
    public synchronized TestEnvironment createTestEnvironment(
            TestParameters Param, PrintWriter log ) throws StatusException {
        XInterface oObj = null;
        XDrawPageSupplier XDPSupp;
        XDrawPage xDP = null;
        XShapes oShapes = null;

        log.println( "creating a test environment" );
        SOfficeFactory SOF = SOfficeFactory.getFactory( (XMultiServiceFactory)Param.getMSF() );
        XDPSupp = (XDrawPageSupplier) UnoRuntime.queryInterface
            (XDrawPageSupplier.class, xTextDoc);
        xDP = XDPSupp.getDrawPage();
        oObj = xDP;

        //dbg.printInterfaces(oObj);
        //System.exit(0);

        log.println( "inserting some Shapes" );
        oShapes = (XShapes) UnoRuntime.queryInterface(XShapes.class, oObj);
        oShapes.add(SOF.createShape(xTextDoc,2000,1500,1000,1000,"Line"));
        oShapes.add(SOF.createShape(xTextDoc,3000,4500,15000,1000,"Ellipse"));
        oShapes.add(SOF.createShape(xTextDoc,5000,3500,7500,5000,"Rectangle"));

        log.println( "creating a new environment for bodytext object" );
        TestEnvironment tEnv = new TestEnvironment( oObj );

        ShapeDsc sDsc = new ShapeDsc(15000,13500,7500,10000,"Rectangle");
        log.println( "adding Shape as mod relation to environment" );
        tEnv.addObjRelation("Shape", new InstCreator( xTextDoc, sDsc));
        tEnv.addObjRelation("DrawPage", oObj);

        return tEnv;
    } // finish method getTestEnvironment

}    // finish class SwXDrawPage

