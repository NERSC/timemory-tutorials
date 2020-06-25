# timemory-run custom components

This example walks through the construction and usage of custom components for timemory. A custom component can utilize the static polymorphic base class `tim::component::base` to inherit many features but ultimately, the goal is to not require a specific base class. The `tim::component::base class` provides the integration into the API and requires two template parameters:
1. component type (i.e. itself)
2. the data type being stored