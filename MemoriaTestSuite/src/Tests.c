/*
 * Tests.c
 *
 *  Created on: 25/10/2015
 *      Author: utnso
 */
#include "Tests.h"

void correrTests(){

	context (complete_example) {

	    describe("Tests de prueba") {


	        before {

	        } end

	        after {

	        } end

			it("Test Funciona cspec"){

	        	should_bool(true) be truthy;

	        }end

	    }end

	}
	printf("\n\n");

}

