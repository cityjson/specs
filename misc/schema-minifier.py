import json
from pathlib import Path
from urllib.request import urlopen

import jsonref  # works with jsonref >= 1.1.0


def unproxy(obj):
    """Recursively convert jsonref proxy objects to plain Python objects."""
    if isinstance(obj, jsonref.JsonRef):
        # Dereference the proxy and recurse
        return unproxy(obj.__subject__)
    elif isinstance(obj, dict):
        return {k: unproxy(v) for k, v in obj.items()}
    elif isinstance(obj, list):
        return [unproxy(item) for item in obj]
    else:
        return obj


def load_schema_with_refs(schema_path: Path) -> dict:
    """Load a JSON schema and resolve all $ref references."""
    base_uri = schema_path.parent.as_uri() + "/"

    def loader(uri):
        """Custom loader for file:// URIs."""
        if uri.startswith("file://"):
            with urlopen(uri) as f:
                return json.load(f)
        # For relative refs, jsonref handles them with base_uri
        with open(uri) as f:
            return json.load(f)

    with open(schema_path) as f:
        schema = jsonref.load(f, base_uri=base_uri, loader=loader)

    # Convert proxy objects to plain dicts
    return unproxy(schema)


#----------------------------
# cityjson.schema.json
#----------------------------
root_schema = Path(__file__).parent.parent / "schemas" / "cityjson.schema.json"

js = load_schema_with_refs(root_schema)

# -- output stitched_schema (for debugging)
temp_path = Path(__file__).parent / "temp.json"
with open(temp_path, "w") as f:
    json.dump(js, f, indent=2)

# -- output minified schema
min_path = Path(__file__).parent.parent / "schemas" / "cityjson.min.schema.json"
with open(min_path, "w") as f:
    json.dump(js, f, separators=(",", ":"))


#----------------------------
# cityjsonfeature.schema.json
#----------------------------
root_schema_feature = Path(__file__).parent.parent / "schemas" / "cityjsonfeature.schema.json"

js_feature = load_schema_with_refs(root_schema_feature)

# -- output minified schema
min_path_feature = Path(__file__).parent.parent / "schemas" / "cityjsonfeature.min.schema.json"
with open(min_path_feature, "w") as f:
    json.dump(js_feature, f, separators=(",", ":"))
