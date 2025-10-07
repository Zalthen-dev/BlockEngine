#pragma once

#include <vector>
#include <variant>
#include <memory>
#include <optional>

#include "Object.h"
#include "../core/Signal.h"
#include "../datatypes/Vector3.h"
#include "../datatypes/Color3.h"

struct Attribute {
    std::string Name;
    std::variant<bool, double, std::string, Vector3Game, Color3> Value;
};

struct Instance :public Object {
    //-- Properties --//
    Instance* Parent = nullptr;
    std::vector<Instance*> Children;
    std::vector<Attribute> Attributes;

    bool Archivable = true;
    std::string Name = ClassName;

    //-- Events --//
    Signal AncestryChanged;
    Signal AttributeChanged;
    Signal ChildAdded;
    Signal ChildRemoved;
    Signal DescendantAdded;
    Signal DescendantRemoving;
    Signal Destroying;

    //-- Methods --//
    Instance(const std::string& className = "Instance");
    virtual ~Instance();

    void AddTag(std::string& tag);
    // GetTags();
    bool HasTag(std::string& tag);
    void RemoveTag(std::string& tag);

    std::optional<Attribute> GetAttribute(std::string& attribute);
    std::vector<Attribute> GetAttributes();
    void SetAttribute(std::string& attribute, Attribute value);

    std::optional<Instance*> FindFirstAncestor(std::string& name);
    std::optional<Instance*> FindFirstAncestorOfClass(std::string& className);
    std::optional<Instance*> FindFirstAncestorWhichIsA(std::string& className);
    std::optional<Instance*> FindFirstChild(std::string& name);
    std::optional<Instance*> FindFirstChildOfClass(std::string& name);
    std::optional<Instance*> FindFirstChildWhichIsA(std::string& name);
    std::optional<Instance*> FindFirstDescendant(std::string& name);

    std::vector<Instance*> GetChildren();
    std::vector<Instance*> GetDescendants();

    bool IsAncestorOf(Instance* descendant);
    bool IsDescendantOf(Instance* ancestor);

    void ClearAllChildren();
    void Destroy();
    // Clone();

    void SetParent(Instance* newParent);
    void AddChild(Instance* child);
    void RemoveChild(Instance* child);
};
