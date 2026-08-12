import requests

import requests

def get(url, headers=None):
    if headers is None:
        headers = {'User-Agent': 'Mozilla/5.0'}
    try:
        resp = requests.get(url, headers=headers, timeout=10)
        resp.raise_for_status()  # 4xx/5xx自动抛异常
        return resp
    except requests.exceptions.RequestException as e:
        print(f"GET 请求异常: {e}")
        return None

def post(url, data, headers=None):
    if headers is None:
        headers = {'Content-Type': 'application/x-www-form-urlencoded'}
    try:
        resp = requests.post(url, data, headers=headers, timeout=10)
        resp.raise_for_status()
        return resp
    except requests.exceptions.RequestException as e:
        print(f"POST 请求异常: {e}")
        return None

if __name__ == "__main__":
    print(__file__)
