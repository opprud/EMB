/**
 * @mainpage DP - Dummy project
 *
 * @section intro_sec Introduction to DUmmy project
 * The DP does alot of magic, using the EA LPC2478 board...
 *
 * @image html ea.png "EA LPC2478 OEM"
 * @file
 * @author  Morten Opprud <morten@hih.au.dk>
 * @version 1.0
 * @date    26/10/2010
 *
 *
 * @section DESCRIPTION
 *
 * The dummy driver implements nada, null, zip functionality. \n
 * but serves only as doxygen demo
 */


#define DUMMY_WRITE_ENABLE           6  ///< Set the write enable latch (enable write operations)
#define DUMMY_WRITE_DISABLE          4  ///< Reset the write enable latch (disable write operations)

/**
 * Init the dummy device
 *
 * This function does initialize the dummy HW module
 * It should be called right after sys init....
 *
 * @param  a  - sets the xxx the Idle mode
 * @param  b  - does other magic
 * @return 12 if ok, -1 if fail
 */
int dummy_init(int a, int b)
{
	int c;
	char d;

	// init c
	c = DUMMY_WRITE_ENABLE;

	// Do magic... Add numbers
	c = a+b;

	// TODO: is this good
	d = c;

	// this is a regular comment
	d++;

	//return OK
	return 12;
}
