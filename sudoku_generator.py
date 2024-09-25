import math, random

"""
This was adapted from a GeeksforGeeks article "Program for Sudoku Generator" by Aarti_Rathi and Ankur Trisal
https://www.geeksforgeeks.org/program-sudoku-generator/

"""


class SudokuGenerator:
    '''
	create a sudoku board - initialize class variables and set up the 2D board
	This should initialize:
	self.row_length		- the length of each row
	self.removed_cells	- the total number of cells to be removed
	self.board			- a 2D list of ints to represent the board
	self.box_length		- the square root of row_length

	Parameters:
    row_length is the number of rows/columns of the board (always 9 for this project)
    removed_cells is an integer value - the number of cells to be removed

	Return:
	None
    '''

    def __init__(self, row_length, removed_cells):
        # Initialize all variables
        self.row_length = row_length
        self.removed_cells = removed_cells
        self.board = [[0 for j in range(row_length)] for i in
                      range(row_length)]  # Create an empty row of row_length, row_length times to make a square board
        self.box_length = int(math.sqrt(
            row_length))  # Box length is sqrt of row_length, cast to int so that it is useable in later functions

    '''
	Returns a 2D python list of numbers which represents the board

	Parameters: None
	Return: list[list]
    '''

    def get_board(self):
        return self.board

    '''
	Displays the board to the console
    This is not strictly required, but it may be useful for debugging purposes

	Parameters: None
	Return: None
    '''

    def print_board(self):
        # Prints each item in the board with a space between squares and each row on a new line
        for row in self.board:
            for item in row:
                print(item, end=" ")
            print('\n')

    '''
	Determines if num is contained in the specified row (horizontal) of the board
    If num is already in the specified row, return False. Otherwise, return True

	Parameters:
	row is the index of the row we are checking
	num is the value we are looking for in the row

	Return: boolean
    '''

    def valid_in_row(self, row, num):
        # (Colgan)here i iterate through the sublist (row) and look through each element seeing if the num is there
        # if not, return True, if it is there I return False

        board = self.board[row]
        for i in board:
            if i == num:
                return False

        return True

    '''
	Determines if num is contained in the specified column (vertical) of the board
    If num is already in the specified col, return False. Otherwise, return True

	Parameters:
	col is the index of the column we are checking
	num is the value we are looking for in the column

	Return: boolean
    '''

    def valid_in_col(self, col, num):
        board = self.board
        # (Colgan) Here I did something similar to valid_in_row, iterating through the same index of each sublist (or row)
        # to see if its there using range(self.row_length). Returns False if number is already in column, False if not

        for i in range(self.row_length):
            if self.board[i][col] == num:
                return False
        return True

    '''
	Determines if num is contained in the 3x3 box specified on the board
    If num is in the specified box starting at (row_start, col_start), return False.
    Otherwise, return True

	Parameters:
	row_start and col_start are the starting indices of the box to check
	i.e. the box is from (row_start, col_start) to (row_start+2, col_start+2)
	num is the value we are looking for in the box

	Return: boolean
    '''

    def valid_in_box(self, row_start, col_start, num):
        # Ensure that row_start and col_start are at the beginning of a 3x3 box
        row_start = row_start - row_start % 3
        col_start = col_start - col_start % 3

        for i in range(3):
            for j in range(3):
                if self.board[row_start + i][col_start + j] == num:
                    return False
        return True

    '''
    Determines if it is valid to enter num at (row, col) in the board
    This is done by checking that num is unused in the appropriate, row, column, and box

	Parameters:
	row and col are the row index and col index of the cell to check in the board
	num is the value to test if it is safe to enter in this cell

	Return: boolean
    '''

    def is_valid(self, row, col, num):
        # Check if the number is not in the given row
        if not self.valid_in_row(row, num):
            return False

        # Check if the number is not in the given column
        if not self.valid_in_col(col, num):
            return False

        # Calculate the starting point of the 3x3 box
        box_start_row = row - row % 3
        box_start_col = col - col % 3

        # Check if the number is not in the 3x3 box
        if not self.valid_in_box(box_start_row, box_start_col, num):
            return False

        # If all checks are passed, it's valid to place the number
        return True

    '''
    Fills the specified 3x3 box with values
    For each position, generates a random digit which has not yet been used in the box

	Parameters:
	row_start and col_start are the starting indices of the box to check
	i.e. the box is from (row_start, col_start) to (row_start+2, col_start+2)

	Return: None
    '''

    def fill_box(self, row_start, col_start):
        nums = list(range(1, 10))
        random.shuffle(nums)  # Shuffle numbers to introduce randomness in filling

        def place_number(index):
            if index == 9:  # Base case: all numbers placed
                return True

            row = row_start + index // 3
            col = col_start + index % 3

            for num in nums:
                if self.is_valid(row, col, num):
                    self.board[row][col] = num
                    if place_number(index + 1):
                        return True
                    self.board[row][col] = 0  # Reset cell on backtrack

            return False  # Trigger backtrack if no number is valid

        return place_number(0)

    '''
    Fills the three boxes along the main diagonal of the board
    These are the boxes which start at (0,0), (3,3), and (6,6)

	Parameters: None
	Return: None
    '''

    def fill_diagonal(self):
        size = 9  # Since the Sudoku board is 9x9
        box_size = 3  # Each box is 3x3

        # Fill each of the three diagonal boxes
        for i in range(0, size, box_size):
            # Use the backtracking method to ensure each box is correctly filled
            if not self.fill_box(i, i):
                raise Exception("Failed to fill the diagonal box at start ({}, {})".format(i, i))

    '''
    DO NOT CHANGE
    Provided for students
    Fills the remaining cells of the board
    Should be called after the diagonal boxes have been filled

	Parameters:
	row, col specify the coordinates of the first empty (0) cell

	Return:
	boolean (whether or not we could solve the board)
    '''

    def fill_remaining(self, row, col):
        if (col >= self.row_length and row < self.row_length - 1):
            row += 1
            col = 0
        if row >= self.row_length and col >= self.row_length:
            return True
        if row < self.box_length:
            if col < self.box_length:
                col = self.box_length
        elif row < self.row_length - self.box_length:
            if col == int(row // self.box_length * self.box_length):
                col += self.box_length
        else:
            if col == self.row_length - self.box_length:
                row += 1
                col = 0
                if row >= self.row_length:
                    return True

        for num in range(1, self.row_length + 1):
            if self.is_valid(row, col, num):
                self.board[row][col] = num
                if self.fill_remaining(row, col + 1):
                    return True
                self.board[row][col] = 0
        return False

    '''
    DO NOT CHANGE
    Provided for students
    Constructs a solution by calling fill_diagonal and fill_remaining

	Parameters: None
	Return: None
    '''

    def fill_values(self):
        self.fill_diagonal()
        self.fill_remaining(0, self.box_length)

    '''
    Removes the appropriate number of cells from the board
    This is done by setting some values to 0
    Should be called after the entire solution has been constructed
    i.e. after fill_values has been called

    NOTE: Be careful not to 'remove' the same cell multiple times
    i.e. if a cell is already 0, it cannot be removed again

	Parameters: None
	Return: None
    '''

    def remove_cells(self):
        total_cells = 81  # Total cells in a 9x9 Sudoku board
        cells_removed = 0

        while cells_removed < self.removed_cells:
            row = random.randint(0, 8)
            col = random.randint(0, 8)

            if self.board[row][col] != 0:  # Ensure the cell is not already removed
                self.board[row][col] = 0
                cells_removed += 1

'''
DO NOT CHANGE
Provided for students
Given a number of rows and number of cells to remove, this function:
1. creates a SudokuGenerator
2. fills its values and saves this as the solved state
3. removes the appropriate number of cells
4. returns the representative 2D Python Lists of the board and solution

Parameters:
size is the number of rows/columns of the board (9 for this project)
removed is the number of cells to clear (set to 0)

Return: list[list] (a 2D Python list to represent the board)
'''


def generate_sudoku(size, removed):
    sudoku = SudokuGenerator(size, removed)
    sudoku.fill_values()
    board = sudoku.get_board()
    sudoku.remove_cells()
    board = sudoku.get_board()
    return board