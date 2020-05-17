import rock

def ask_user_for_move(position):
    move = None
    while (not move):
        move = rock.Move.parse(input("Enter a move: "))
        if not move:
            print("Cannot read move")
        elif not move.is_legal(position):
            print("{} is an illegal move for {}".format(move, position.player_to_move))
            move = None
    return move

def play_interactive_game():
    position = rock.starting_position()
    score = 0
    i = 0
    outcome = rock.Ongoing

    while True:
        print("State after {} moves".format(i))
        print(position)
        print("Score: {}".format(score))

        human_move = ask_user_for_move(position)
        position.apply_move(human_move)

        outcome = position.game_outcome()
        if outcome != rock.GameOutcome.Ongoing:
            return outcome, position

        analysis = rock.analyze_position(position)
        print("AI move: {}".format(analysis.best_move))
        position.apply_move(analysis.best_move)

        outcome = position.game_outcome()
        if outcome != rock.GameOutcome.Ongoing:
            return outcome, position

        i += 1

def play_ai_vs_ai(depth_white, depth_black):
    p = rock.starting_position()
    i = 0

    while True:
        print("After {} turns".format(i))
        print(p)

        if p.player_to_move == rock.Player.White:
            depth = depth_white 
        else:
            depth = depth_black
        analysis = rock.analyze_position(p, depth)
        p.apply_move(analysis.best_move)
        i += 1

        print("Analysis: {}".format(analysis))

        outcome = p.game_outcome()
        print(outcome)
        if outcome != rock.GameOutcome.Ongoing:
            return outcome, p

if __name__ == "__main__":
    outcome, final_position = play_interactive_game()

    print("Final position:")
    print(final_position)
    print("Result: {}".format(outcome))
