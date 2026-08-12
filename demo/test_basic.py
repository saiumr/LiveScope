import print_color

chat = {'text': '有点吊！[dog][妙][dog][捂脸][妙]'}
emots = {

'[dog]': {'count': 2, 'descript': '[dog]', 'emoji': '[dog]', 'emoticon_id': 208, 'emoticon_unique': 'emoji_208', 'height': 20, 'url': 'http://i0.hdslb.com/bfs/live/4428c84e694fbf4e0ef6c06e958d9352c3582740.png', 'width': 20}, 

'[妙]': {'count': 2, 'descript': '[妙]', 'emoji': '[妙]', 'emoticon_id': 210, 'emoticon_unique': 'emoji_210', 'height': 20, 'url': 'http://i0.hdslb.com/bfs/live/08f735d950a0fba267dda140673c9ab2edf6410d.png', 'width': 20}, 

'[捂脸]': {'count': 1, 'descript': '[捂脸]', 'emoji': '[捂脸]', 'emoticon_id': 222, 'emoticon_unique': 'emoji_222', 'height': 20, 'url': 'http://i0.hdslb.com/bfs/live/e6073c6849f735ae6cb7af3a20ff7dcec962b4c5.png', 'width': 20}
}

chat_text = chat['text']
if emots is not None:
    for key in emots:
        chat_text = chat_text.replace(key, str(print_color.YELLOW) + key + str(print_color.RESET), emots[key]['count'])

print(str(print_color.WHITE_BRIGHT) + chat_text + str(print_color.RESET))


