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

def pick_ai_move_and_evaluate(position):
    print("Deciding on a move...")
    analysis = rock.analyze_position(position)
    print("{}".format(analysis.best_move))
    return analysis

def play_interactive_game():
    position = rock.starting_position
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

        analysis = pick_ai_move_and_evaluate(position)
        position.apply_move(analysis.best_move)

        outcome = position.game_outcome()
        if outcome != rock.GameOutcome.Ongoing:
            return outcome, position

        i += 1

if __name__ == "__main__":
    outcome, final_position = play_interactive_game()

    print("Final position:")
    print(final_position)
    print("Result: {}".format(outcome))
