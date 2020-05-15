import rock
import random

def get_random_pos(position=rock.starting_position(), num_iterations=20):
    for i in range(0, num_iterations):
        moves = rock.list_moves(position)
        if len(moves) == 0:
            i -= 1
            continue
        move = random.choice(moves)
        position.apply_move(move)
    return position

if __name__ == "__main__":
    print('--- 5 ---')
    for i in range(0, 10):
        print("{:e }".format(get_random_pos(num_iterations=5)))
    print('--- 10 ---')
    for i in range(0, 10):
        print("{:e }".format(get_random_pos(num_iterations=10)))
    print('--- 20 ---')
    for i in range(0, 10):
        print("{:e }".format(get_random_pos(num_iterations=20)))
    print('--- 30 ---')
    for i in range(0, 10):
        print("{:e }".format(get_random_pos(num_iterations=30)))
