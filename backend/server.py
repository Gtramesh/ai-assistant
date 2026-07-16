import os
import json
import shutil
import subprocess
import platform
import psutil
from datetime import datetime
from pathlib import Path
from flask import Flask, request, jsonify, send_from_directory
from flask_cors import CORS
import requests

app = Flask(__name__, static_folder="../frontend", static_url_path="")
CORS(app)

HISTORY_FILE = "chat_history.json"
DEFAULT_ROOT = os.path.expanduser("~")


def is_local(request):
    host = request.host.split(":")[0]
    return host in ("127.0.0.1", "localhost", "::1")


def require_local():
    if not is_local(request):
        return jsonify({"error": "File access is only available on your local computer."}), 403
    return None


def load_history():
    if os.path.exists(HISTORY_FILE):
        with open(HISTORY_FILE, "r", encoding="utf-8") as f:
            return json.load(f)
    return []


def save_history(history):
    with open(HISTORY_FILE, "w", encoding="utf-8") as f:
        json.dump(history, f, ensure_ascii=False, indent=2)


@app.route("/")
def index():
    return send_from_directory(app.static_folder, "index.html")


@app.route("/<path:path>")
def static_files(path):
    return send_from_directory(app.static_folder, path)


@app.route("/api/files", methods=["GET"])
def list_files():
    denied = require_local()
    if denied: return denied
    path = request.args.get("path", DEFAULT_ROOT)
    path = os.path.expanduser(path)
    if not os.path.exists(path):
        return jsonify({"error": "Path not found"}), 404
    entries = []
    try:
        for entry in sorted(os.scandir(path), key=lambda e: (not e.is_dir(), e.name.lower())):
            try:
                stat = entry.stat()
                entries.append({
                    "name": entry.name,
                    "path": entry.path,
                    "is_dir": entry.is_dir(),
                    "size": stat.st_size if not entry.is_dir() else None,
                    "modified": datetime.fromtimestamp(stat.st_mtime).isoformat(),
                    "extension": os.path.splitext(entry.name)[1] if not entry.is_dir() else None,
                })
            except PermissionError:
                entries.append({"name": entry.name, "path": entry.path, "is_dir": entry.is_dir(), "size": None, "modified": None, "extension": None, "error": "permission denied"})
    except PermissionError:
        return jsonify({"error": "Permission denied"}), 403
    return jsonify({"path": path, "parent": os.path.dirname(path), "entries": entries})


@app.route("/api/file", methods=["GET"])
def read_file():
    denied = require_local()
    if denied: return denied
    path = request.args.get("path", "")
    path = os.path.expanduser(path)
    if not os.path.exists(path):
        return jsonify({"error": "File not found"}), 404
    if os.path.isdir(path):
        return jsonify({"error": "Path is a directory"}), 400
    try:
        size = os.path.getsize(path)
        if size > 5 * 1024 * 1024:
            return jsonify({"error": "File too large (>5MB)"}), 400
        with open(path, "r", encoding="utf-8", errors="replace") as f:
            content = f.read()
        return jsonify({"path": path, "content": content, "size": size})
    except Exception as e:
        return jsonify({"error": str(e)}), 500


@app.route("/api/file", methods=["POST"])
def write_file():
    denied = require_local()
    if denied: return denied
    data = request.get_json()
    path = data.get("path", "")
    content = data.get("content", "")
    path = os.path.expanduser(path)
    try:
        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, "w", encoding="utf-8") as f:
            f.write(content)
        return jsonify({"success": True, "path": path})
    except Exception as e:
        return jsonify({"error": str(e)}), 500


@app.route("/api/file", methods=["DELETE"])
def delete_file():
    denied = require_local()
    if denied: return denied
    path = request.args.get("path", "")
    path = os.path.expanduser(path)
    if not os.path.exists(path):
        return jsonify({"error": "File not found"}), 404
    try:
        if os.path.isdir(path):
            shutil.rmtree(path)
        else:
            os.remove(path)
        return jsonify({"success": True})
    except Exception as e:
        return jsonify({"error": str(e)}), 500


@app.route("/api/file", methods=["PUT"])
def rename_file():
    denied = require_local()
    if denied: return denied
    data = request.get_json()
    old_path = data.get("old_path", "")
    new_path = data.get("new_path", "")
    old_path = os.path.expanduser(old_path)
    new_path = os.path.expanduser(new_path)
    try:
        os.rename(old_path, new_path)
        return jsonify({"success": True})
    except Exception as e:
        return jsonify({"error": str(e)}), 500


@app.route("/api/mkdir", methods=["POST"])
def make_dir():
    denied = require_local()
    if denied: return denied
    data = request.get_json()
    path = data.get("path", "")
    path = os.path.expanduser(path)
    try:
        os.makedirs(path, exist_ok=True)
        return jsonify({"success": True, "path": path})
    except Exception as e:
        return jsonify({"error": str(e)}), 500


@app.route("/api/search", methods=["GET"])
def search_files():
    denied = require_local()
    if denied: return denied
    query = request.args.get("q", "").lower()
    root = request.args.get("path", DEFAULT_ROOT)
    root = os.path.expanduser(root)
    results = []
    max_results = 50
    try:
        for dirpath, dirnames, filenames in os.walk(root):
            for name in filenames + dirnames:
                if query in name.lower():
                    full = os.path.join(dirpath, name)
                    results.append({"name": name, "path": full, "is_dir": os.path.isdir(full)})
                    if len(results) >= max_results:
                        return jsonify({"results": results, "truncated": True})
            if len(results) >= max_results:
                break
    except PermissionError:
        pass
    return jsonify({"results": results, "truncated": False})


@app.route("/api/access", methods=["GET"])
def check_access():
    return jsonify({"local": is_local(request)})


@app.route("/api/system", methods=["GET"])
def system_info():
    return jsonify({
        "platform": platform.system(),
        "platform_version": platform.version(),
        "hostname": platform.node(),
        "python": platform.python_version(),
        "cpu_count": psutil.cpu_count(),
        "ram_total_gb": round(psutil.virtual_memory().total / (1024**3), 1),
        "ram_used_gb": round(psutil.virtual_memory().used / (1024**3), 1),
        "ram_percent": psutil.virtual_memory().percent,
        "current_user": os.getenv("USERNAME") or os.getenv("USER", "unknown"),
        "cwd": os.getcwd(),
    })


@app.route("/api/chat", methods=["POST"])
def chat():
    data = request.get_json()
    message = data.get("message", "")
    language = data.get("language", "en-US")
    history = load_history()

    result = process_command(message, language)

    history.append({"role": "user", "content": message})
    history.append({"role": "assistant", "content": result["response"]})
    if len(history) > 100:
        history = history[-100:]
    save_history(history)

    return jsonify({
        "response": result["response"],
        "action": result.get("action"),
        "data": result.get("data"),
    })


@app.route("/api/history", methods=["GET"])
def get_history():
    return jsonify(load_history())


@app.route("/api/history", methods=["DELETE"])
def clear_history():
    save_history([])
    return jsonify({"success": True})


def process_command(message, language="en-US"):
    msg = message.lower().strip()

    if any(w in msg for w in ["list files", "show files", "browse", "open folder", "show directory", "ls"]):
        import re
        path_match = re.search(r'(?:in|from|at|of)\s+(.+?)(?:\s*$)', message, re.IGNORECASE)
        path = path_match.group(1) if path_match else DEFAULT_ROOT
        path = os.path.expanduser(path.strip('"').strip("'"))
        if not os.path.exists(path):
            return {"response": f"Path not found: {path}"}
        try:
            entries = []
            for entry in sorted(os.scandir(path), key=lambda e: (not e.is_dir(), e.name.lower())):
                prefix = "[DIR] " if entry.is_dir() else "      "
                entries.append(f"{prefix}{entry.name}")
            return {"response": f"Contents of {path}:\n\n" + "\n".join(entries[:50]), "action": "list_files", "data": {"path": path}}
        except PermissionError:
            return {"response": f"Permission denied: {path}"}

    if any(w in msg for w in ["read file", "open file", "show file", "cat "]):
        import re
        path_match = re.search(r'(?:read|open|show|cat)\s+(?:file\s+)?(.+?)(?:\s*$)', message, re.IGNORECASE)
        if not path_match:
            return {"response": "Please specify a file path. Example: 'read file C:\\Users\\me\\doc.txt'"}
        path = path_match.group(1).strip().strip('"').strip("'")
        path = os.path.expanduser(path)
        if not os.path.exists(path):
            return {"response": f"File not found: {path}"}
        if os.path.isdir(path):
            return {"response": f"Path is a directory, not a file: {path}"}
        try:
            size = os.path.getsize(path)
            if size > 5 * 1024 * 1024:
                return {"response": f"File too large ({size // 1024}KB). Use the file browser to view it."}
            with open(path, "r", encoding="utf-8", errors="replace") as f:
                content = f.read()
            return {"response": f"Content of {path}:\n\n```\n{content}\n```", "action": "read_file", "data": {"path": path, "content": content}}
        except Exception as e:
            return {"response": f"Error reading file: {e}"}

    if any(w in msg for w in ["write file", "create file", "save file", "new file"]):
        import re
        path_match = re.search(r'(?:write|create|save|new)\s+(?:file\s+)?(\S+)', message, re.IGNORECASE)
        content_match = re.search(r'(?:with content|content|containing|:\s*)(.+?)(?:\s*$)', message, re.IGNORECASE)
        if not path_match:
            return {"response": "Please specify a file path. Example: 'create file hello.txt with content Hello World'"}
        path = os.path.expanduser(path_match.group(1).strip('"').strip("'"))
        content = content_match.group(1) if content_match else ""
        try:
            os.makedirs(os.path.dirname(path) or ".", exist_ok=True)
            with open(path, "w", encoding="utf-8") as f:
                f.write(content)
            return {"response": f"File created: {path}", "action": "write_file", "data": {"path": path}}
        except Exception as e:
            return {"response": f"Error creating file: {e}"}

    if any(w in msg for w in ["delete file", "remove file", "rm "]):
        import re
        path_match = re.search(r'(?:delete|remove|rm)\s+(?:file\s+)?(.+?)(?:\s*$)', message, re.IGNORECASE)
        if not path_match:
            return {"response": "Please specify a file path. Example: 'delete file test.txt'"}
        path = os.path.expanduser(path_match.group(1).strip().strip('"').strip("'"))
        if not os.path.exists(path):
            return {"response": f"File not found: {path}"}
        try:
            os.remove(path)
            return {"response": f"File deleted: {path}"}
        except Exception as e:
            return {"response": f"Error deleting file: {e}"}

    if any(w in msg for w in ["search", "find file", "find "]):
        import re
        query_match = re.search(r'(?:search|find(?:\s+file)?)\s+(?:for\s+)?(.+?)(?:\s*$)', message, re.IGNORECASE)
        if not query_match:
            return {"response": "What would you like to search for?"}
        query = query_match.group(1).strip().strip('"').strip("'")
        results = []
        try:
            for dirpath, dirnames, filenames in os.walk(DEFAULT_ROOT):
                for name in filenames + dirnames:
                    if query.lower() in name.lower():
                        full = os.path.join(dirpath, name)
                        results.append(full)
                        if len(results) >= 20:
                            break
                if len(results) >= 20:
                    break
        except PermissionError:
            pass
        if results:
            return {"response": f"Found {len(results)} results:\n\n" + "\n".join(results), "action": "search", "data": {"query": query, "results": results}}
        return {"response": f"No files found matching '{query}'"}

    if any(w in msg for w in ["help", "what can you do", "commands", "how to"]):
        help_text = """Here's what I can do:

**Chat:**
- Ask me anything in Tamil or English!
- I can help with coding, explanations, translations, and more.

**Voice:**
- Click the mic button to speak
- Toggle Tamil/English with the language button
- Enable "Speak" to hear responses read aloud

**Tips:**
- Try asking "what is Python?" or "explain AI"
- Switch to Tamil and ask "உன்னால் என்ன செய்ய முடியும்?"
- Use voice input for hands-free conversation!"""
        return {"response": help_text}

    try:
        response = query_ai(message, language)
        return {"response": response}
    except Exception as e:
        return {"response": f"AI Error: {str(e)}\n\nI can still help with file operations! Try 'help' to see what I can do."}


def query_ai(message, language="en-US"):
    errors = []
    lang_name = "Tamil" if language.startswith("ta") else "English"
    system_prompt = f"You are a helpful AI assistant running locally. Respond in {lang_name}. Be concise and helpful."

    try:
        resp = requests.post(
            "https://text.pollinations.ai/",
            json={
                "messages": [
                    {"role": "system", "content": system_prompt},
                    {"role": "user", "content": message}
                ],
                "model": "openai",
                "seed": 42,
            },
            timeout=60,
        )
        if resp.status_code == 200:
            text = resp.text.strip()
            if text:
                return text
        errors.append(f"Pollinations returned {resp.status_code}")
    except Exception as e:
        errors.append(f"Pollinations: {e}")

    try:
        resp = requests.post(
            "https://api-inference.huggingface.co/models/HuggingFaceH4/zephyr-7b-beta",
            json={
                "inputs": f"<|system|>\n{system_prompt}<|end|>\n<|user|>\n{message}<|end|>\n<|assistant|>",
                "parameters": {"max_new_tokens": 512, "temperature": 0.7, "return_full_text": False},
            },
            headers={"Content-Type": "application/json"},
            timeout=30,
        )
        if resp.status_code == 200:
            result = resp.json()
            if isinstance(result, list) and len(result) > 0:
                return result[0].get("generated_text", "No response.")
        errors.append(f"HuggingFace returned {resp.status_code}")
    except Exception as e:
        errors.append(f"HuggingFace: {e}")

    return f"AI services unavailable. Errors: {'; '.join(errors)}\n\nI can still help with file operations - try 'help'!"


if __name__ == "__main__":
    print("=" * 50)
    print("  AI Assistant - Local Server")
    print("=" * 50)
    print(f"  Server:  http://localhost:5000")
    print(f"  System:  {platform.system()} {platform.release()}")
    print(f"  User:    {os.getenv('USERNAME') or os.getenv('USER', 'unknown')}")
    print("=" * 50)
    print("  Open your browser and go to: http://localhost:5000")
    print("  Press Ctrl+C to stop the server")
    print("=" * 50)
    app.run(host="127.0.0.1", port=5000, debug=False)
