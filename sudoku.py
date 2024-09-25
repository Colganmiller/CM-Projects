import pygame as pg
import sys
from sudoku_generator import generate_sudoku

def main_menu(screen):
    # Initialize Font
    title_font = pg.font.Font(None, 100)
    sub_font = pg.font.Font(None, 75)
    button_font = pg.font.Font(None, 60)

    # Initialize Colors
    bg_color = (60, 55, 68)
    text_color = (251, 255, 241)
    button_color = (48, 102, 190)

    # Render text to screen
    screen.fill(bg_color)
    title_surface = title_font.render("Sudoku", True, text_color)
    title_rectangle = title_surface.get_rect(center=(900 // 2, 900 // 2 - 250))
    sub_surface = sub_font.render("Pick a difficulty:", True, text_color)
    sub_rect = sub_surface.get_rect(center=(900 // 2, 900 // 2))

    screen.blit(title_surface, title_rectangle)
    screen.blit(sub_surface, sub_rect)

    buttons = ["Easy", "Medium", "Hard"]
    button_rects = []
    for i, text in enumerate(buttons):
        text_surface = button_font.render(text, True, text_color)
        button_surface = pg.Surface((text_surface.get_width() + 20, text_surface.get_height() + 20))
        button_surface.fill(button_color)
        button_surface.blit(text_surface, (10, 10))
        rect = button_surface.get_rect(center=(900 // 2, 900 // 2 + 100 + i * 100))
        screen.blit(button_surface, rect)
        button_rects.append(rect)

    pg.display.flip()

    while True:
        for event in pg.event.get():
            if event.type == pg.QUIT:
                pg.quit()
                sys.exit()
            if event.type == pg.MOUSEBUTTONDOWN:
                for i, rect in enumerate(button_rects):
                    if rect.collidepoint(event.pos):
                        return 30 + 10 * i  # 30 for Easy, 40 for Medium, 50 for Hard
                    
def win_screen(screen):

    title_font = pg.font.Font(None, 100)
    sub_font = pg.font.Font(None, 75)
    button_font = pg.font.Font(None, 60)

    # Initialize Colors
    bg_color = (60, 55,68)
    text_color = (251, 255, 241)
    button_color = (48, 102, 190)

    # Render text to screen
    screen.fill(bg_color)

    title_surface = title_font.render("Game Won!", True, text_color)
    title_rectangle = title_surface.get_rect(center=(900 // 2, 900 // 2 - 250))
    exit_text = button_font.render("Exit", 0, text_color)
    button_surface = pg.Surface((title_surface.get_width() + 20, title_surface.get_height() + 20))
    button_surface.fill(button_color)
    screen.blit(title_surface, title_rectangle)
    buttons = ["Exit"]
    button_rects = []
    for i, text in enumerate(buttons):
        text_surface = button_font.render(text, True, text_color)
        button_surface = pg.Surface((text_surface.get_width() + 20, text_surface.get_height() + 20))
        button_surface.fill(button_color)
        button_surface.blit(text_surface, (10, 10))
        rect = button_surface.get_rect(center=(900 // 2, 900 // 2 + 100 + i * 100))
        screen.blit(button_surface, rect)
        button_rects.append(rect)

    #button_surface.blit(exit_text, (10,10))
    pg.display.flip()

    while True:
        for event in pg.event.get():
            if event.type == pg.QUIT:
                pg.quit()
                sys.exit()
            if event.type == pg.MOUSEBUTTONDOWN:
                pg.quit()
                sys.exit()

def loss_screen(screen):

    title_font = pg.font.Font(None, 100)
    sub_font = pg.font.Font(None, 75)
    button_font = pg.font.Font(None, 60)

    # Initialize Colors
    bg_color = (60, 55,68)
    text_color = (251, 255, 241)
    button_color = (48, 102, 190)

    # Render text to screen
    screen.fill(bg_color)

    title_surface = title_font.render("You lost :(", True, text_color)
    title_rectangle = title_surface.get_rect(center=(900 // 2, 900 // 2 - 250))
    restart_text = button_font.render("Restart", 0, text_color)
    button_surface = pg.Surface((title_surface.get_width() + 20, title_surface.get_height() + 20))
    button_surface.fill(button_color)
    screen.blit(title_surface, title_rectangle)
    buttons = ["Restart"]
    button_rects = []
    for i, text in enumerate(buttons):
        text_surface = button_font.render(text, True, text_color)
        button_surface = pg.Surface((text_surface.get_width() + 20, text_surface.get_height() + 20))
        button_surface.fill(button_color)
        button_surface.blit(text_surface, (10, 10))
        rect = button_surface.get_rect(center=(900 // 2, 900 // 2 + 100 + i * 100))
        screen.blit(button_surface, rect)
        button_rects.append(rect)

    #button_surface.blit(exit_text, (10,10))
    pg.display.flip()

    while True:
        for event in pg.event.get():
            if event.type == pg.QUIT:
                pg.quit()
                sys.exit()
            if event.type == pg.MOUSEBUTTONDOWN:
                main()


def is_solved_correctly(board):
    expected = set(range(1, 10))

    # Check rows
    for row in board:
        if set(row) != expected:
            return False

    # Check columns
    for col in range(9):
        column = [board[row][col] for row in range(9)]
        if set(column) != expected:
            return False

    # Check 3x3 squares
    for row in range(0, 9, 3):
        for col in range(0, 9, 3):
            block = [board[x][y] for x in range(row, row + 3) for y in range(col, col + 3)]
            if set(block) != expected:
                return False

    return True


def play_screen(cells_removed, screen):
    board = generate_sudoku(9, cells_removed)
    original_board = [row[:] for row in board]  # Copy of the original board
    font = pg.font.Font(None, 40)
    CELL_SIZE = 60
    GRID_ORIGIN = (50, 50)
    BLACK = (0, 0, 0)
    GREY = (200, 200, 200)
    selected_cell = None

    def board_is_full(board):
        return all(cell != 0 for row in board for cell in row)

    def draw_grid():
        for x in range(10):
            line_thickness = 4 if x % 3 == 0 else 1
            pg.draw.line(screen, BLACK, (GRID_ORIGIN[0] + x * CELL_SIZE, GRID_ORIGIN[1]),
                         (GRID_ORIGIN[0] + x * CELL_SIZE, GRID_ORIGIN[1] + 9 * CELL_SIZE), line_thickness)
            pg.draw.line(screen, BLACK, (GRID_ORIGIN[0], GRID_ORIGIN[1] + x * CELL_SIZE),
                         (GRID_ORIGIN[0] + 9 * CELL_SIZE, GRID_ORIGIN[1] + x * CELL_SIZE), line_thickness)

    def draw_numbers():
        for i in range(9):
            for j in range(9):
                cell_value = board[i][j]
                if cell_value != 0:
                    text = font.render(str(cell_value), True, BLACK)
                    screen.blit(text, (GRID_ORIGIN[0] + j * CELL_SIZE + 20, GRID_ORIGIN[1] + i * CELL_SIZE + 15))

    def draw_selected():
        if selected_cell:
            pg.draw.rect(screen, (255, 0, 0), (GRID_ORIGIN[0] + selected_cell[1] * CELL_SIZE, GRID_ORIGIN[1] + selected_cell[0] * CELL_SIZE, CELL_SIZE, CELL_SIZE), 3)

    def handle_mouse_click(pos):
        if pos[0] < GRID_ORIGIN[0] or pos[1] < GRID_ORIGIN[1] or pos[0] > GRID_ORIGIN[0] + 9 * CELL_SIZE or pos[1] > GRID_ORIGIN[1] + 9 * CELL_SIZE:
            return None
        row = (pos[1] - GRID_ORIGIN[1]) // CELL_SIZE
        col = (pos[0] - GRID_ORIGIN[0]) // CELL_SIZE
        return (row, col)

    def handle_arrow_buttons(selected_cell, direction):
        row, col = selected_cell
        if direction == 'up' and row > 0:
            row -= 1
        elif direction == 'down' and row < 8:
            row += 1
        elif direction == 'left' and col > 0:
            col -= 1
        elif direction == 'right' and col < 8:
            col += 1

        return (row, col)

    def draw_buttons():
        buttons = ["Reset", "Restart", "Exit"]
        button_actions = []
        for i, button in enumerate(buttons):
            button_rect = pg.Rect(50, 600 + i * 50, 200, 40)
            pg.draw.rect(screen, (180, 180, 255), button_rect)
            text = font.render(button, True, BLACK)
            screen.blit(text, (50 + 10, 600 + i * 50 + 10))
            button_actions.append((button_rect, button))
        return button_actions

    running = True
    while running:
        screen.fill(GREY)
        draw_grid()
        draw_numbers()
        draw_selected()
        button_actions = draw_buttons()
        for event in pg.event.get():
            if event.type == pg.QUIT:
                running = False
            elif event.type == pg.MOUSEBUTTONDOWN:
                pos = pg.mouse.get_pos()
                selected_cell = handle_mouse_click(pos)
                for button_rect, action in button_actions:
                    if button_rect.collidepoint(pos):
                        if action == "Reset":
                            board = [row[:] for row in original_board]
                        elif action == "Restart":
                            main()
                        elif action == "Exit":
                            pg.quit()
                            sys.exit()
            elif event.type == pg.KEYDOWN and selected_cell:
                if event.unicode.isdigit():
                    num = int(event.unicode)
                    if original_board[selected_cell[0]][selected_cell[1]] == 0:  # Only modify empty cells
                        board[selected_cell[0]][selected_cell[1]] = num
                        if board_is_full(board):  # Check if the board is full
                            if is_solved_correctly(board):
                                win_screen(screen)
                                running = False  # Optionally show a win screen here
                            else:

                                loss_screen(screen)
                                main()

                if event.key == pg.K_UP:
                    selected_cell = handle_arrow_buttons(selected_cell, 'up')
                elif event.key == pg.K_LEFT:
                    selected_cell = handle_arrow_buttons(selected_cell, 'left')
                elif event.key == pg.K_RIGHT:
                    selected_cell = handle_arrow_buttons(selected_cell, 'right')
                elif event.key == pg.K_DOWN:
                    selected_cell = handle_arrow_buttons(selected_cell, 'down')

        pg.display.flip()

def main():
    pg.init()
    game = pg.display.set_mode((900, 900))
    pg.display.set_caption('Sudoku')
    cells_removed = main_menu(game)
    play_screen(cells_removed, game)
    pg.quit()

if __name__ == '__main__':
    main()
