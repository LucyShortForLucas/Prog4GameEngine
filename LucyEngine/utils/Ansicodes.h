#pragma once

namespace ansi {

// Reset
constexpr auto RESET = "\033[0m";

// Foreground - regular
constexpr auto BLACK = "\033[30m";
constexpr auto RED = "\033[31m";
constexpr auto GREEN = "\033[32m";
constexpr auto YELLOW = "\033[33m";
constexpr auto BLUE = "\033[34m";
constexpr auto MAGENTA = "\033[35m";
constexpr auto CYAN = "\033[36m";
constexpr auto WHITE = "\033[37m";

// Foreground - bold
constexpr auto BLACK_BOLD = "\033[1;30m";
constexpr auto RED_BOLD = "\033[1;31m";
constexpr auto GREEN_BOLD = "\033[1;32m";
constexpr auto YELLOW_BOLD = "\033[1;33m";
constexpr auto BLUE_BOLD = "\033[1;34m";
constexpr auto MAGENTA_BOLD = "\033[1;35m";
constexpr auto CYAN_BOLD = "\033[1;36m";
constexpr auto WHITE_BOLD = "\033[1;37m";

// Foreground - bright
constexpr auto BLACK_BRIGHT = "\033[90m";
constexpr auto RED_BRIGHT = "\033[91m";
constexpr auto GREEN_BRIGHT = "\033[92m";
constexpr auto YELLOW_BRIGHT = "\033[93m";
constexpr auto BLUE_BRIGHT = "\033[94m";
constexpr auto MAGENTA_BRIGHT = "\033[95m";
constexpr auto CYAN_BRIGHT = "\033[96m";
constexpr auto WHITE_BRIGHT = "\033[97m";

// Foreground - bold bright
constexpr auto BLACK_BOLD_BRIGHT = "\033[1;90m";
constexpr auto RED_BOLD_BRIGHT = "\033[1;91m";
constexpr auto GREEN_BOLD_BRIGHT = "\033[1;92m";
constexpr auto YELLOW_BOLD_BRIGHT = "\033[1;93m";
constexpr auto BLUE_BOLD_BRIGHT = "\033[1;94m";
constexpr auto MAGENTA_BOLD_BRIGHT = "\033[1;95m";
constexpr auto CYAN_BOLD_BRIGHT = "\033[1;96m";
constexpr auto WHITE_BOLD_BRIGHT = "\033[1;97m";

// Background - regular
constexpr auto BG_BLACK = "\033[40m";
constexpr auto BG_RED = "\033[41m";
constexpr auto BG_GREEN = "\033[42m";
constexpr auto BG_YELLOW = "\033[43m";
constexpr auto BG_BLUE = "\033[44m";
constexpr auto BG_MAGENTA = "\033[45m";
constexpr auto BG_CYAN = "\033[46m";
constexpr auto BG_WHITE = "\033[47m";

// Background - bright
constexpr auto BG_BLACK_BRIGHT = "\033[100m";
constexpr auto BG_RED_BRIGHT = "\033[101m";
constexpr auto BG_GREEN_BRIGHT = "\033[102m";
constexpr auto BG_YELLOW_BRIGHT = "\033[103m";
constexpr auto BG_BLUE_BRIGHT = "\033[104m";
constexpr auto BG_MAGENTA_BRIGHT = "\033[105m";
constexpr auto BG_CYAN_BRIGHT = "\033[106m";
constexpr auto BG_WHITE_BRIGHT = "\033[107m";

}