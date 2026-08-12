# 前景色
BLACK = '\033[30m'
RED = '\033[31m'
GREEN = '\033[32m'
YELLOW = '\033[33m'
BLUE = '\033[34m'
PURPLE = '\033[35m'
CYAN = '\033[36m'
WHITE = '\033[37m'
LIGHT_GRAY = "\033[90m"   # 浅灰
PINK = "\033[95m"         # 粉紫色/粉色（终端标准粉色）
SOFT_PINK = "\033[38;2;255;180;220m"           # 柔和粉色
BOLD_SOFT_PINK = "\033[1;38;2;255;180;220m"    # 加粗高亮柔和嫩粉

# 高亮亮色
LRED = '\033[91m'
LGREEN = '\033[92m'
LYELLOW = '\033[93m'
LBLUE = '\033[94m'
WHITE_BRIGHT = "\033[97m" # 亮白色
# 柔和亮灰
GRAY_BRIGHT        = "\033[38;2;200;200;200m"
GRAY_BRIGHT_BOLD   = "\033[1;38;2;200;200;200m"

# 效果
BOLD = '\033[1m'   # 加粗
UNDERLINE = '\033[4m' # 下划线
RESET = '\033[0m'  # 重置


if __name__ == '__main__':
    # 示例打印
    print(f"{RED}红色文字{RESET}")
    print(f"{GREEN}{BOLD}绿色加粗{RESET}")
    print(f"{LYELLOW}{UNDERLINE}亮黄下划线{RESET}")
    print(f"{BLUE}普通蓝色 {PURPLE}紫色混排{RESET}")
    print(f"{LIGHT_GRAY}浅灰色文字{RESET}")
    print(f"{WHITE_BRIGHT}亮白色文字{RESET}")
    print(f"{PINK}粉色文字{RESET}")
    print(f"{SOFT_PINK}柔和嫩粉色{RESET}")
    print(f"{BOLD}{LGREEN}cola{GRAY_BRIGHT}: {BOLD_SOFT_PINK}@小孩曾卓君 {WHITE_BRIGHT}你打不过我你信不信？{YELLOW}[dog]{RESET}")
