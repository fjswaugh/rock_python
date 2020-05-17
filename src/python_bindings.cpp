#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <rock/algorithms.h>
#include <rock/fen.h>
#include <rock/format.h>
#include <rock/parse.h>
#include <rock/starting_position.h>
#include <stdint.h>
#include <algorithm>

using namespace pybind11::literals;

namespace
{

auto py_apply_move_board(rock::Board& x, rock::Move m) -> void
{
    auto const player = x[rock::Player::White].at(m.from) ? rock::Player::White
                                                          : rock::Player::Black;

    x = rock::apply_move(m, x, player);
}

auto py_apply_move_position(rock::Position& x, rock::Move m) -> void
{
    x = rock::apply_move(m, x);
}

auto py_format_position(rock::Position const& x, std::string const& format_string) -> std::string
{
    auto const full_format_string = format_string.empty() ? "{}" : "{:" + format_string + "}";
    return fmt::format(full_format_string, x);
}

auto py_format_board(rock::Board const& x, std::string const& format_string) -> std::string
{
    auto const full_format_string = format_string.empty() ? "{}" : "{:" + format_string + "}";
    return fmt::format(full_format_string, x);
}

auto py_board_at_1(rock::Board const& b, rock::BoardCoordinates c) -> std::optional<rock::Player>
{
    for (auto p : {rock::Player::White, rock::Player::Black})
        if (b[p].at(c))
            return p;
    return std::nullopt;
}

auto py_board_at_2(rock::Board const& b, rock::BoardCoordinates c, rock::Player p) -> bool
{
    return b[p].at(c);
}

}  // namespace

PYBIND11_MODULE(rock, m)
{
    m.doc() = "Rock library for Romanian Checkers";

    // Types

    pybind11::enum_<rock::Player>(m, "Player")
        .value("White", rock::Player::White)
        .value("Black", rock::Player::Black)
        .export_values();

    pybind11::enum_<rock::GameOutcome>(m, "GameOutcome")
        .value("Ongoing", rock::GameOutcome::Ongoing)
        .value("WhiteWins", rock::GameOutcome::WhiteWins)
        .value("BlackWins", rock::GameOutcome::BlackWins)
        .value("Draw", rock::GameOutcome::Draw)
        .export_values();

    pybind11::class_<rock::BoardCoordinates>(m, "BoardCoordinates")
        .def(pybind11::init<>())
        .def(pybind11::init(
            [](std::string const& str) { return rock::parse_board_coordinates(str).value(); }))
        .def(pybind11::init([](int x, int y) {
            return rock::BoardCoordinates{x, y};
        }))
        .def_static(
            "parse", [](std::string const& str) { return rock::parse_board_coordinates(str); })
        .def("__str__", [](rock::BoardCoordinates x) { return to_string(x); })
        .def("__repr__", [](rock::BoardCoordinates x) {
            return fmt::format("rock.BoardCoordinates('{}')", x);
        });

    pybind11::class_<rock::Move>(m, "Move")
        .def(pybind11::init<>())
        .def(pybind11::init([](std::string const& str) { return rock::parse_move(str).value(); }))
        .def(pybind11::init([](rock::BoardCoordinates from, rock::BoardCoordinates to) {
            return rock::Move{from, to};
        }))
        .def_readonly("from_coord", &rock::Move::from)
        .def_readonly("to_coord", &rock::Move::to)
        .def("is_legal", &rock::is_move_legal, "position"_a)
        .def_static("parse", [](std::string const& str) { return rock::parse_move(str); })
        .def("__str__", [](rock::Move x) { return to_string(x); })
        .def("__repr__", [](rock::Move x) { return fmt::format("rock.Move('{}')", x); });

    pybind11::class_<rock::Board>(m, "Board")
        .def(pybind11::init<>())
        .def_static(
            "parse_fen", [](std::string const& str) { return rock::parse_fen_to_board(str); })
        .def("to_fen", [](rock::Board const& b) { return rock::format_as_fen(b); })
        .def("at", &py_board_at_1, "coordinates"_a)
        .def("at", &py_board_at_2, "coordinates"_a, "player"_a)
        .def("apply_move", &py_apply_move_board, "move"_a)
        .def("__format__", &py_format_board)
        .def("__str__", [](rock::Board const& x) { return to_string(x); });

    pybind11::class_<rock::Position>(m, "Position")
        .def(pybind11::init<>())
        .def_static(
            "parse_fen", [](std::string const& str) { return rock::parse_fen_to_position(str); })
        .def("to_fen", [](rock::Position const& p) { return rock::format_as_fen(p); })
        .def_property("board", &rock::Position::board, &rock::Position::set_board)
        .def_property(
            "player_to_move", &rock::Position::player_to_move, &rock::Position::set_player_to_move)
        .def("apply_move", &py_apply_move_position, "move"_a)
        .def("game_outcome", &rock::get_game_outcome)
        .def("list_moves", &rock::list_moves)
        .def("__format__", &py_format_position)
        .def("__str__", [](rock::Position const& x) { return to_string(x); });

    pybind11::class_<rock::PositionAnalysis>(m, "PositionAnalysis")
        .def(pybind11::init<>())
        .def_readonly("best_move", &rock::PositionAnalysis::best_move)
        .def_readonly("score", &rock::PositionAnalysis::score)
        .def_readonly("principal_variation", &rock::PositionAnalysis::principal_variation)
        .def("__repr__", [](rock::PositionAnalysis const& x) {
            return fmt::format(
                "(best_move={}, score={}, principal_variation=[{}])",
                x.best_move ? to_string(*x.best_move) : "-",
                x.score,
                fmt::join(x.principal_variation.begin(), x.principal_variation.end(), ", "));
        });

    // Functions

    m.def("starting_position", [] { return rock::starting_position; });

    m.def("list_moves", &rock::list_moves, "position"_a);

    m.def("count_moves", &rock::count_moves, "position"_a = rock::starting_position, "level"_a = 1);

    m.def(
        "analyze_position",
        [](rock::Position const& position, int depth) {
            return rock::analyze_position(position, depth);
        },
        "position"_a,
        "depth"_a = 6);
}
