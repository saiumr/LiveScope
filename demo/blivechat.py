import readapi
import time
import print_color

# ref blog: https://www.cnblogs.com/addPath/p/15743000.html
# url = api + room_id
api = "http://api.live.bilibili.com/ajax/msg?roomid="

def get_chat(room_id=None):
    if room_id is None:
        print("%s get_chat 没有room id!" % __file__)
        return
    url = api + room_id
    resp = readapi.get(url).json()
    return resp

def print_room_chat(room_id=None):
    resp = get_chat(room_id)
    old_data = resp['data']['room']
    time.sleep(3)
    while 1:
        resp = get_chat(room_id)
        data = resp['data']['room']
        old_data_id_set = {only_id["id_str"] for only_id in old_data}
        diff_data = [item for item in data if item["id_str"] not in old_data_id_set]
        for t in diff_data:
            # 昵称和冒号（绿 + 灰）
            nick_common = str(print_color.LGREEN) + str(print_color.BOLD) + t['nickname'] + str(print_color.GRAY_BRIGHT_BOLD) + ": " + str(print_color.RESET)
            # 弹幕文本高亮（白） + 表情高亮（黄）
            chat_text = t['text']
            emots = t['emots']
            if emots is not None:
                for key in emots:
                    chat_text = chat_text.replace(key, str(print_color.YELLOW) + key + str(print_color.RESET), emots[key]['count'])
            # @消息高亮
            at_msg = ""
            at_other = t['reply']['reply_uname']
            if at_other:
                at_msg = str(print_color.BOLD_SOFT_PINK) + "@" + at_other + str(print_color.RESET) + " "
            print(nick_common + at_msg + str(print_color.WHITE_BRIGHT) + chat_text + str(print_color.RESET))
        old_data = data
        time.sleep(0.1)

def print_one_chat(room_id=None):
    resp = get_chat(room_id=room_id)
    print(resp)

room_id = "242067"      # 大都督
# room_id = "21720131"    # 点点
# room_id = "789280"      # 蜥蜴君

if __name__ == '__main__':
    print_room_chat(room_id=room_id)
    # print_one_chat(room_id=room_id)
