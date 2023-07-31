#pragma once
#include "Renderer/Framebuffer.h"

namespace TS_ENGINE {

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;

		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		Vector4 ReadPixelColor(uint32_t attachmentIndex, int x, int y);

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index) const override;

		virtual const FramebufferSpecification& GetSpecification() const override;

	private:
		uint32_t mRendererID = 0;
		FramebufferSpecification mSpecification;

		std::vector<FramebufferTextureSpecification> mColorAttachmentSpecifications;
		FramebufferTextureSpecification mDepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> mColorAttachments;
		uint32_t mDepthAttachment = 0;
	};
}
