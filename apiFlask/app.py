from flask import Flask, jsonify, request
from flask_cors import CORS  # Adicionando CORS

app = Flask(__name__)
CORS(app)  # Habilita CORS para todas as rotas

estado = {"estado": -1, "modo": "automatico"}  # Estado inicial


@app.route("/toggle_bomba", methods=["GET", "POST"])
def toggle_bomba():
    global estado

    if request.method == "POST":
        novo_estado = request.get_json()
        if novo_estado and "estado" in novo_estado:
            estado["estado"] = novo_estado["estado"]
            estado["modo"] = "manual"
            print(f"Novo estado recebido: {estado}")

    return jsonify(estado)


@app.route("/modo_automatico", methods=["POST"])
def modo_automatico():
    global estado

    estado["modo"] = "automatico"
    estado["estado"] = -1
    return jsonify(estado)


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
