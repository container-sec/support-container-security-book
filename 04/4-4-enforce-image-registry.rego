package user.dockerfile.ID001

# メタデータ
# 検出時のIDや説明などを宣言
__rego_metadata__ = {
  "id": "ID001",
  "title": "Registry is forbidden",
  "severity": "HIGH",
  "type": "Custom Dockerfile Check",
  "description": "Deny anything other than the allowed Docker registry.",
}

# 入力されるデータの指定。Dockerfileを検査するため、“dockerfile”を指定
__rego_input__ = {
  "selector": [
    {"type": "dockerfile"},
  ],
}

allowed_registries = ["hub.example.com"]

deny[msg] {
  op := input.Stages[_].Commands[_]
  op.Cmd == "from"
  not startswith(op.Value[x], allowed_registries[x])
  msg := sprintf("This image registry is forbidden: %s", [op.Value[x]])
}

