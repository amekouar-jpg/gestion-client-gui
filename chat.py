
import requests
import json

invoke_url = "https://integrate.api.nvidia.com/v1/chat/completions"

API_KEY = "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"

headers = {
    "Authorization": f"Bearer {API_KEY}",
    "Accept": "text/event-stream",
    "Content-Type": "application/json"
}


def extract_text(delta):
    """
    Kimi/NVIDIA peut renvoyer soit:
    - content (réponse normale)
    - reasoning_content (raisonnement interne)
    """
    if not isinstance(delta, dict):
        return None

    return delta.get("content") or delta.get("reasoning_content")


def chat():
    messages = []

    print("AI Chat ready. Type 'exit' to quit.\n")

    while True:
        user_input = input("You: ")

        if user_input.lower().strip() == "exit":
            break

        messages.append({"role": "user", "content": user_input})

        payload = {
            "model": "moonshotai/kimi-k2.6",
            "messages": messages,
            "max_tokens": 4096,
            "temperature": 0.7,
            "top_p": 1,
            "stream": True,
            "chat_template_kwargs": {"thinking": False}  # évite bruit reasoning
        }

        try:
            response = requests.post(
                invoke_url,
                headers=headers,
                json=payload,
                stream=True,
                timeout=60
            )
        except Exception as e:
            print("Request error:", e)
            continue

        if response.status_code != 200:
            print("HTTP Error:", response.status_code)
            print(response.text)
            continue

        print("AI: ", end="", flush=True)

        assistant_reply = ""

        for line in response.iter_lines():
            if not line:
                continue

            try:
                line = line.decode("utf-8")
            except:
                continue

            if not line.startswith("data: "):
                continue

            data = line[6:].strip()

            if data == "[DONE]":
                break

            try:
                chunk = json.loads(data)

                delta = chunk.get("choices", [{}])[0].get("delta", {})
                text = extract_text(delta)

                if text:
                    print(text, end="", flush=True)
                    assistant_reply += text

            except:
                continue

        print("\n")

        if assistant_reply:
            messages.append({"role": "assistant", "content": assistant_reply})


if __name__ == "__main__":
    chat()