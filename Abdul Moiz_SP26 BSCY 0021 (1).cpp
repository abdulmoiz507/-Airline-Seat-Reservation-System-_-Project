/*
 * ============================================================
 *  Airline Seat Reservation System
 *  Course Assignment – Functions and Arrays (C++)
 * ============================================================
 *
 *  SEAT LAYOUT  (12 rows × 7 seats = 84 seats total)
 *  -------------------------------------------------
 *  Rows 0-1   (14 seats) ? First Class    – Rs. 18,000
 *  Rows 2-3   (14 seats) ? Business Class – Rs. 14,000
 *  Rows 4-11  (56 seats) ? Economy Class  – Rs. 10,000
 *
 *  Convention: 0 = available, 1 = reserved
 * ============================================================
 */

#include <iostream>
using namespace std;

// --- Constants --------------------------------------------
const int ROWS        = 12;
const int COLS        = 7;

// Row boundaries (inclusive start, exclusive end)
const int FC_START    = 0;   // First Class rows
const int FC_END      = 2;
const int BC_START    = 2;   // Business Class rows
const int BC_END      = 4;
const int EC_START    = 4;   // Economy Class rows
const int EC_END      = 12;

// Fares
const int FARE_FIRST    = 18000;
const int FARE_BUSINESS = 14000;
const int FARE_ECONOMY  = 10000;

// --- Function Prototypes ----------------------------------
void  displayMenu();
bool  bookFirstClass   (int Seats[][7], int rows, int cols, int noOfSeats);
bool  bookBusinessClass(int Seats[][7], int rows, int cols, int noOfSeats);
bool  bookEconomyClass (int Seats[][7], int rows, int cols, int noOfSeats);
int   checkAvailableSeats(int Seats[][7], int rows, int cols);
void  getFare();
void  viewSeatingPlan  (int Seats[][7], int rows, int cols);
bool  systemReset      (int Seats[][7], int rows, int cols);

// Helper: counts available seats within a specific row range
int availableInRange(int Seats[][7], int startRow, int endRow, int cols);

// Helper: books 'needed' seats in rows [startRow, endRow)
// Returns true if enough seats were available and booked.
bool bookInRange(int Seats[][7], int startRow, int endRow,
                 int cols, int noOfSeats);

// ----------------------------------------------------------
//  MAIN
// ----------------------------------------------------------
int main() {

    // Declare and initialise the 2-D seat array to all zeros
    int Seats[ROWS][COLS] = {0};

    int choice;

    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin  >> choice;
        cout << endl;

        if (choice == 1) {
            // -- Book First Class --------------------------
            int n;
            cout << "Enter number of First Class seats to book: ";
            cin  >> n;

            if (bookFirstClass(Seats, ROWS, COLS, n))
                cout << "\n[SUCCESS] " << n
                     << " First Class seat(s) booked successfully.\n";
            else
                cout << "\n[FAILED] Could not complete First Class booking."
                        " Not enough available seats.\n";

        } else if (choice == 2) {
            // -- Book Business Class -----------------------
            int n;
            cout << "Enter number of Business Class seats to book: ";
            cin  >> n;

            if (bookBusinessClass(Seats, ROWS, COLS, n))
                cout << "\n[SUCCESS] " << n
                     << " Business Class seat(s) booked successfully.\n";
            else
                cout << "\n[FAILED] Could not complete Business Class booking."
                        " Not enough available seats.\n";

        } else if (choice == 3) {
            // -- Book Economy Class ------------------------
            int n;
            cout << "Enter number of Economy Class seats to book: ";
            cin  >> n;

            if (bookEconomyClass(Seats, ROWS, COLS, n))
                cout << "\n[SUCCESS] " << n
                     << " Economy Class seat(s) booked successfully.\n";
            else
                cout << "\n[FAILED] Could not complete Economy Class booking."
                        " Not enough available seats.\n";

        } else if (choice == 4) {
            // -- View Seating Plan -------------------------
            viewSeatingPlan(Seats, ROWS, COLS);

        } else if (choice == 5) {
            // -- View Fares --------------------------------
            getFare();

        } else if (choice == 6) {
            // -- System Reset ------------------------------
            if (systemReset(Seats, ROWS, COLS))
                cout << "[SUCCESS] All seats have been reset to available.\n";
            else
                cout << "[FAILED] System reset could not be completed.\n";

        } else if (choice == 7) {
            // -- Exit --------------------------------------
            cout << "Thank you for using the Airplane Reservation System. "
                    "Goodbye!\n";

        } else {
            cout << "[ERROR] Invalid choice. Please select 1-7.\n";
        }

        cout << endl;

    } while (choice != 7);

    return 0;
}

// ----------------------------------------------------------
//  displayMenu  –  prints the main menu (matches Figure 1)
// ----------------------------------------------------------
void displayMenu() {
    cout << "--------------------------------------------------\n";
    cout << "      Welcome to Airplane Reservation System      \n";
    cout << "--------------------------------------------------\n";
    cout << "1. Book First Class Seats\n";
    cout << "2. Book Business Class Seats\n";
    cout << "3. Book Economy Class Seats\n";
    cout << "4. View Airplane Seating Plan\n";
    cout << "5. View Fare(s)\n";
    cout << "6. System Reset\n";
    cout << "7. Exit from The Booking System\n";
    cout << "--------------------------------------------------\n";
}

// ----------------------------------------------------------
//  availableInRange  –  HELPER
//  Counts free (0) seats between startRow (inclusive)
//  and endRow (exclusive).
//
//  Efficiency note: a single nested loop over only the
//  relevant rows avoids scanning the whole array.
// ----------------------------------------------------------
int availableInRange(int Seats[][7], int startRow, int endRow, int cols) {
    int count = 0;
    for (int r = startRow; r < endRow; r++)
        for (int c = 0; c < cols; c++)
            if (Seats[r][c] == 0)
                count++;
    return count;
}

// ----------------------------------------------------------
//  bookInRange  –  HELPER
//  Attempts to mark 'noOfSeats' seats as reserved (1)
//  within rows [startRow, endRow).
//  Seats are filled left-to-right, row-by-row.
//
//  Returns true  ? booking succeeded (enough seats existed)
//  Returns false ? not enough available seats; no change made
//
//  Efficiency note: we check availability BEFORE modifying
//  the array so we never need to "undo" partial bookings.
// ----------------------------------------------------------
bool bookInRange(int Seats[][7], int startRow, int endRow,
                 int cols, int noOfSeats) {

    // Pre-check: are there enough free seats?
    if (availableInRange(Seats, startRow, endRow, cols) < noOfSeats)
        return false;   // not enough seats – return false immediately

    // Book seats one by one until we have filled 'noOfSeats'
    int booked = 0;
    for (int r = startRow; r < endRow && booked < noOfSeats; r++) {
        for (int c = 0; c < cols && booked < noOfSeats; c++) {
            if (Seats[r][c] == 0) {
                Seats[r][c] = 1;
                booked++;
            }
        }
    }
    return true;
}

// ----------------------------------------------------------
//  bookFirstClass
//  Books 'noOfSeats' seats in rows 0-1 (First Class).
// ----------------------------------------------------------
bool bookFirstClass(int Seats[][7], int rows, int cols, int noOfSeats) {
    // Validate input: must book at least 1 seat
    if (noOfSeats <= 0) return false;

    return bookInRange(Seats, FC_START, FC_END, cols, noOfSeats);
}

// ----------------------------------------------------------
//  bookBusinessClass
//  Books 'noOfSeats' seats in rows 2-3 (Business Class).
// ----------------------------------------------------------
bool bookBusinessClass(int Seats[][7], int rows, int cols, int noOfSeats) {
    if (noOfSeats <= 0) return false;

    return bookInRange(Seats, BC_START, BC_END, cols, noOfSeats);
}

// ----------------------------------------------------------
//  bookEconomyClass
//  Books 'noOfSeats' seats in rows 4-11 (Economy Class).
// ----------------------------------------------------------
bool bookEconomyClass(int Seats[][7], int rows, int cols, int noOfSeats) {
    if (noOfSeats <= 0) return false;

    return bookInRange(Seats, EC_START, EC_END, cols, noOfSeats);
}

// ----------------------------------------------------------
//  checkAvailableSeats
//  Returns total number of free seats across the whole plane.
// ----------------------------------------------------------
int checkAvailableSeats(int Seats[][7], int rows, int cols) {
    return availableInRange(Seats, 0, rows, cols);
}

// ----------------------------------------------------------
//  getFare  –  prints fare information for all classes
// ----------------------------------------------------------
void getFare() {
    cout << "--------------------------------------------------\n";
    cout << "               Fare Information                  \n";
    cout << "--------------------------------------------------\n";
    cout << "  First Class    (Rows  1- 2) : Rs. " << FARE_FIRST    << "\n";
    cout << "  Business Class (Rows  3- 4) : Rs. " << FARE_BUSINESS << "\n";
    cout << "  Economy Class  (Rows  5-12) : Rs. " << FARE_ECONOMY  << "\n";
    cout << "--------------------------------------------------\n";
}

// ----------------------------------------------------------
//  viewSeatingPlan
//  Displays a 12×7 grid.
//    0 ? available (printed as '0')
//    1 ? reserved  (printed as '1')
//  Row and seat labels are included for readability.
//  A class separator is drawn between row groups.
// ----------------------------------------------------------
void viewSeatingPlan(int Seats[][7], int rows, int cols) {
    cout << "\n--------------------------------------------------\n";
    cout << "               Seating Plan                      \n";
    cout << "   (0 = Available,  1 = Reserved)                \n";
    cout << "--------------------------------------------------\n";
    cout << "     Seat: ";
    for (int c = 0; c < cols; c++)
        cout << " " << (c + 1);
    cout << "\n";

    for (int r = 0; r < rows; r++) {

        // Print class label at the start of each section
        if (r == FC_START)
            cout << "  -- First Class --\n";
        else if (r == BC_START)
            cout << "  -- Business Class --\n";
        else if (r == EC_START)
            cout << "  -- Economy Class --\n";

        // Print row number (1-indexed for display)
        cout << "  Row " ;
        if (r + 1 < 10) cout << " ";   // padding for single-digit rows
        cout << (r + 1) << ":  ";

        for (int c = 0; c < cols; c++)
            cout << Seats[r][c] << " ";

        cout << "\n";
    }

    // Summary line
    int available = checkAvailableSeats(Seats, rows, cols);
    int reserved  = (rows * cols) - available;
    cout << "--------------------------------------------------\n";
    cout << "  Total seats : " << (rows * cols) << "\n";
    cout << "  Reserved    : " << reserved      << "\n";
    cout << "  Available   : " << available     << "\n";
    cout << "--------------------------------------------------\n";
}

// ----------------------------------------------------------
//  systemReset
//  Sets every element of the 2-D array back to 0.
//  Returns true on success.
// ----------------------------------------------------------
bool systemReset(int Seats[][7], int rows, int cols) {
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            Seats[r][c] = 0;

    return true;   // reset always succeeds
}
