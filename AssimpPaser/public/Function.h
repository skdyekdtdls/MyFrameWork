#pragma once

fs::path ToResourcesDir()
{
    fs::path ResourcesDir = fs::current_path();
    ResourcesDir = ResourcesDir.parent_path().parent_path();
    ResourcesDir.append("Client\\Bin\\Resources\\");


    return ResourcesDir;
}

fs::path ToModelsDir()
{
    fs::path ModelsDir = ToResourcesDir();
    ModelsDir.append("Models\\");

    return ModelsDir;
}
